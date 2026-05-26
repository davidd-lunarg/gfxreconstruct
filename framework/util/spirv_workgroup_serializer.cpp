/*
** Copyright (c) 2026 LunarG, Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
*/

#include "spirv_workgroup_serializer.h"

#include "spirv/unified1/spirv.hpp"

#include <initializer_list>
#include <unordered_map>
#include <utility>
#include <vector>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(util)

static constexpr uint32_t kSpirvMagic       = 0x07230203;
static constexpr size_t   kSpirvHeaderWords = 5;

static bool IsAtomicOpcode(uint32_t op)
{
    switch (op)
    {
        case spv::OpAtomicLoad:
        case spv::OpAtomicStore:
        case spv::OpAtomicExchange:
        case spv::OpAtomicCompareExchange:
        case spv::OpAtomicCompareExchangeWeak:
        case spv::OpAtomicIIncrement:
        case spv::OpAtomicIDecrement:
        case spv::OpAtomicIAdd:
        case spv::OpAtomicISub:
        case spv::OpAtomicSMin:
        case spv::OpAtomicUMin:
        case spv::OpAtomicSMax:
        case spv::OpAtomicUMax:
        case spv::OpAtomicAnd:
        case spv::OpAtomicOr:
        case spv::OpAtomicXor:
        case spv::OpAtomicFMinEXT:
        case spv::OpAtomicFMaxEXT:
        case spv::OpAtomicFAddEXT:
            return true;
        default:
            return false;
    }
}

bool ContainsAtomicOps(const uint32_t* words, size_t word_count)
{
    if (words == nullptr || word_count < kSpirvHeaderWords)
    {
        return false;
    }
    if (words[0] != kSpirvMagic)
    {
        return false;
    }

    size_t pos = kSpirvHeaderWords;
    while (pos < word_count)
    {
        uint32_t inst_word = words[pos];
        uint32_t wc        = inst_word >> 16;
        uint32_t op        = inst_word & 0xFFFFu;
        if (wc == 0 || pos + wc > word_count)
        {
            break;
        }
        if (IsAtomicOpcode(op))
        {
            return true;
        }
        pos += wc;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Internal SPIR-V helpers for the workgroup-serialization patcher.
//-----------------------------------------------------------------------------

namespace
{

constexpr uint32_t MakeInstWord(uint32_t op, uint32_t word_count)
{
    return ((word_count & 0xFFFFu) << 16) | (op & 0xFFFFu);
}

// Walk SPIR-V word stream, calling fn(pos, op, operand_count, operand_ptr) for
// each instruction. fn returns false to abort.
template <typename Fn>
void ForEachInstruction(const uint32_t* words, size_t word_count, Fn fn)
{
    size_t pos = kSpirvHeaderWords;
    while (pos < word_count)
    {
        uint32_t inst_word = words[pos];
        uint32_t wc        = inst_word >> 16;
        uint32_t op        = inst_word & 0xFFFFu;
        if (wc == 0 || pos + wc > word_count)
        {
            break;
        }
        const uint32_t  operand_count = wc - 1;
        const uint32_t* operands      = words + pos + 1;
        if (!fn(pos, op, operand_count, operands))
        {
            return;
        }
        pos += wc;
    }
}

// Append one instruction (opcode + operands) to `out`. word_count includes
// the leading instruction word.
void EmitInst(std::vector<uint32_t>& out, uint32_t op, std::initializer_list<uint32_t> operands)
{
    uint32_t wc = static_cast<uint32_t>(1 + operands.size());
    out.push_back(MakeInstWord(op, wc));
    for (uint32_t w : operands) out.push_back(w);
}

void EmitInstWithOperands(std::vector<uint32_t>& out,
                          uint32_t               op,
                          const uint32_t*        operands,
                          uint32_t               operand_count)
{
    uint32_t wc = 1 + operand_count;
    out.push_back(MakeInstWord(op, wc));
    out.insert(out.end(), operands, operands + operand_count);
}

inline uint32_t AlignUp(uint32_t v, uint32_t align)
{
    return (v + align - 1) & ~(align - 1);
}

//-----------------------------------------------------------------------------
// Parsed metadata for an input module.
//-----------------------------------------------------------------------------

struct ModuleMetadata
{
    bool      is_compute             = false;
    bool      local_size_spec_const  = false;   // OpExecutionModeId LocalSizeId: punt
    uint32_t  entry_point_id         = 0;
    uint32_t  bound                  = 0;
    uint32_t  local_size[3]          = { 1, 1, 1 };
    size_t    entry_point_inst_pos   = 0;       // word position of the OpEntryPoint
    uint32_t  entry_point_word_count = 0;

    // BuiltIn Input variable IDs (0 if not present in the module).
    uint32_t  wgid_var_id = 0;
    uint32_t  gid_var_id  = 0;
    uint32_t  lid_var_id  = 0;
    // WorkGroupSize built-in constant ID (rarely used directly).

    // Common type/constant IDs we need to reuse or create.
    uint32_t  uint_type_id        = 0;
    uint32_t  uvec3_type_id       = 0;
    uint32_t  uint_ptr_input_id   = 0;          // OpTypePointer Input uint
    uint32_t  uvec3_ptr_input_id  = 0;          // OpTypePointer Input uvec3
    uint32_t  const_uint_0_id     = 0;
    uint32_t  const_uint_1_id     = 0;
    uint32_t  const_uint_2_id     = 0;

    // The largest byte offset reached by any member of any push-constant
    // struct already declared. New synth members must start at or after
    // AlignUp(this, 16).
    uint32_t  existing_pc_extent_bytes = 0;
};

// Information about a load/access-chain site that ultimately produces a
// value derived from WorkgroupId or GlobalInvocationId. We use these to
// re-bind the original result id via OpCopyObject inserted immediately
// after the original instruction.
struct RedirectSite
{
    enum class Source
    {
        Wgid,
        Gid,
    };
    enum class Form
    {
        FullLoad,      // OpLoad uvec3 %var          -> redirect to synth_uvec3
        ComponentLoad, // OpLoad uint  %ptr_from_AC  -> redirect to synth_component
    };

    size_t   inst_pos        = 0; // word position of the existing instruction
    uint32_t inst_op         = 0;
    uint32_t orig_result_id  = 0;
    Source   source          = Source::Wgid;
    Form     form            = Form::FullLoad;
    uint32_t component_index = 0; // 0,1,2 (only valid for ComponentLoad)
};

//-----------------------------------------------------------------------------
// Metadata extraction.
//-----------------------------------------------------------------------------

bool ExtractMetadata(const uint32_t* words, size_t word_count, ModuleMetadata& md)
{
    md.bound = words[3];

    // Constants table (used for resolving access-chain indices and type sizes).
    struct IntConst { uint32_t type_id; uint32_t value; };
    std::unordered_map<uint32_t, IntConst> int_constants;
    std::unordered_map<uint32_t, std::pair<uint32_t, bool>> int_types; // id -> (width_bits, signed)
    // Pointer types: id -> (storage_class, pointee_type_id)
    std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> ptr_types;
    // Vector types: id -> (component_type, count)
    std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> vec_types;
    // Struct types: id -> list of member type ids
    std::unordered_map<uint32_t, std::vector<uint32_t>> struct_members;
    // Variables: id -> (pointer-type-id, storage-class)
    std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> variables;
    // Built-in decoration on variable: var_id -> BuiltIn enum value.
    std::unordered_map<uint32_t, uint32_t> builtin_of_var;
    // Per-struct member offsets: struct_id -> { member_idx -> offset }.
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>> struct_offsets;
    // Per-struct member sizes (derived).
    std::unordered_map<uint32_t, uint32_t> type_size_bytes;

    ForEachInstruction(words, word_count, [&](size_t pos, uint32_t op, uint32_t oc, const uint32_t* o) {
        switch (op)
        {
        case spv::OpEntryPoint:
            if (oc >= 2)
            {
                if (o[0] == spv::ExecutionModelGLCompute)
                {
                    md.is_compute             = true;
                    md.entry_point_id         = o[1];
                    md.entry_point_inst_pos   = pos;
                    md.entry_point_word_count = 1 + oc;
                }
            }
            break;
        case spv::OpExecutionMode:
            if (oc >= 5 && o[1] == spv::ExecutionModeLocalSize)
            {
                md.local_size[0] = o[2];
                md.local_size[1] = o[3];
                md.local_size[2] = o[4];
            }
            break;
        case spv::OpExecutionModeId:
            if (oc >= 5 && o[1] == spv::ExecutionModeLocalSizeId)
            {
                md.local_size_spec_const = true;
            }
            break;

        case spv::OpTypeInt:
            if (oc >= 3)
            {
                int_types[o[0]] = { o[1], o[2] != 0 };
                if (o[1] == 32 && o[2] == 0 && md.uint_type_id == 0)
                {
                    md.uint_type_id = o[0];
                }
                if (o[1] == 32)
                {
                    type_size_bytes[o[0]] = 4;
                }
            }
            break;
        case spv::OpTypeFloat:
            if (oc >= 2 && o[1] == 32)
            {
                type_size_bytes[o[0]] = 4;
            }
            break;
        case spv::OpTypeVector:
            if (oc >= 3)
            {
                vec_types[o[0]] = { o[1], o[2] };
                auto it = type_size_bytes.find(o[1]);
                if (it != type_size_bytes.end())
                {
                    type_size_bytes[o[0]] = it->second * o[2];
                }
                if (o[1] == md.uint_type_id && o[2] == 3 && md.uvec3_type_id == 0)
                {
                    md.uvec3_type_id = o[0];
                }
            }
            break;
        case spv::OpTypeMatrix:
            if (oc >= 3)
            {
                auto it = type_size_bytes.find(o[1]);
                if (it != type_size_bytes.end())
                {
                    type_size_bytes[o[0]] = it->second * o[2];
                }
            }
            break;
        case spv::OpTypeStruct:
            if (oc >= 1)
            {
                std::vector<uint32_t> members(o + 1, o + oc);
                struct_members[o[0]] = std::move(members);
            }
            break;
        case spv::OpTypePointer:
            if (oc >= 3)
            {
                ptr_types[o[0]] = { o[1], o[2] };
                if (o[1] == spv::StorageClassInput)
                {
                    if (md.uvec3_type_id != 0 && o[2] == md.uvec3_type_id && md.uvec3_ptr_input_id == 0)
                    {
                        md.uvec3_ptr_input_id = o[0];
                    }
                    if (md.uint_type_id != 0 && o[2] == md.uint_type_id && md.uint_ptr_input_id == 0)
                    {
                        md.uint_ptr_input_id = o[0];
                    }
                }
            }
            break;

        case spv::OpConstant:
            if (oc >= 3)
            {
                int_constants[o[1]] = { o[0], o[2] };
                if (md.uint_type_id != 0 && o[0] == md.uint_type_id)
                {
                    if (o[2] == 0 && md.const_uint_0_id == 0) md.const_uint_0_id = o[1];
                    if (o[2] == 1 && md.const_uint_1_id == 0) md.const_uint_1_id = o[1];
                    if (o[2] == 2 && md.const_uint_2_id == 0) md.const_uint_2_id = o[1];
                }
            }
            break;

        case spv::OpVariable:
            if (oc >= 3)
            {
                variables[o[1]] = { o[0], o[2] };
            }
            break;

        case spv::OpDecorate:
            if (oc >= 3 && o[1] == spv::DecorationBuiltIn)
            {
                builtin_of_var[o[0]] = o[2];
            }
            break;
        case spv::OpMemberDecorate:
            if (oc >= 4 && o[2] == spv::DecorationOffset)
            {
                struct_offsets[o[0]][o[1]] = o[3];
            }
            break;
        default:
            break;
        }
        return true;
    });

    if (!md.is_compute) return false;
    if (md.local_size_spec_const) return false;

    // Identify built-in Input variables of interest.
    for (auto& kv : builtin_of_var)
    {
        const uint32_t var_id = kv.first;
        const uint32_t bi     = kv.second;
        auto var_it = variables.find(var_id);
        if (var_it == variables.end()) continue;
        if (var_it->second.second != spv::StorageClassInput) continue;
        if (bi == spv::BuiltInWorkgroupId)       md.wgid_var_id = var_id;
        else if (bi == spv::BuiltInGlobalInvocationId) md.gid_var_id  = var_id;
        else if (bi == spv::BuiltInLocalInvocationId)  md.lid_var_id  = var_id;
    }

    // Compute existing push-constant extent: over every variable in
    // PushConstant storage class, look at its pointee struct's member
    // offsets and add the member's type size.
    for (auto& kv : variables)
    {
        const uint32_t ptr_id  = kv.second.first;
        const uint32_t storage = kv.second.second;
        if (storage != spv::StorageClassPushConstant) continue;
        auto pt = ptr_types.find(ptr_id);
        if (pt == ptr_types.end()) continue;
        const uint32_t pointee = pt->second.second;
        auto sm = struct_members.find(pointee);
        if (sm == struct_members.end()) continue;
        auto offs = struct_offsets.find(pointee);
        if (offs == struct_offsets.end()) continue;
        const auto& members  = sm->second;
        for (uint32_t midx = 0; midx < members.size(); ++midx)
        {
            auto oit = offs->second.find(midx);
            if (oit == offs->second.end()) continue;
            uint32_t off = oit->second;
            // Pessimistic: if we can't resolve member type size, assume 16 bytes.
            uint32_t sz = 16;
            auto tsz_it = type_size_bytes.find(members[midx]);
            if (tsz_it != type_size_bytes.end()) sz = tsz_it->second;
            uint32_t end = off + sz;
            if (end > md.existing_pc_extent_bytes)
            {
                md.existing_pc_extent_bytes = end;
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Emission helpers.
//-----------------------------------------------------------------------------

// Insert a uint32_t at a specific position in OpEntryPoint's interface list.
// The interface list begins after operands[0]=execution_model, operands[1]=id,
// then a literal name (variable-length string), then the interface ids.
//
// We emit the (possibly-extended) entry-point by copying its operand range
// and appending new interface IDs. Returns the number of new operands added.
uint32_t WriteEntryPointPossiblyExtended(std::vector<uint32_t>&     out,
                                        const uint32_t*            words,
                                        const ModuleMetadata&      md,
                                        const std::vector<uint32_t>& extra_interface_ids)
{
    // Words at md.entry_point_inst_pos: [inst_word, exec_model, entry_id, name..., iface..., ]
    const uint32_t  wc        = md.entry_point_word_count;
    const uint32_t* base      = words + md.entry_point_inst_pos;
    const uint32_t  orig_oc   = wc - 1;
    const uint32_t* orig_ops  = base + 1;

    uint32_t new_oc = orig_oc + static_cast<uint32_t>(extra_interface_ids.size());
    out.push_back(MakeInstWord(spv::OpEntryPoint, 1 + new_oc));
    out.insert(out.end(), orig_ops, orig_ops + orig_oc);
    for (uint32_t id : extra_interface_ids) out.push_back(id);
    return static_cast<uint32_t>(extra_interface_ids.size());
}

} // namespace

//-----------------------------------------------------------------------------
// Public patcher.
//-----------------------------------------------------------------------------

bool PatchForWorkgroupSerialization(const uint32_t*            words,
                                    size_t                     word_count,
                                    WorkgroupSerializerOutput& out)
{
    if (words == nullptr || word_count < kSpirvHeaderWords) return false;
    if (words[0] != kSpirvMagic) return false;

    ModuleMetadata md;
    if (!ExtractMetadata(words, word_count, md)) return false;

    // If the module reads neither WorkgroupId nor GlobalInvocationId, there
    // is nothing for us to redirect; the serialised-dispatch loop will run
    // the original module unchanged.
    const bool need_wgid = md.wgid_var_id != 0;
    const bool need_gid  = md.gid_var_id  != 0;
    if (!need_wgid && !need_gid)
    {
        // Tell the caller "no patch needed; serialised dispatch can still
        // run with no push-constant content". Emit the original module
        // verbatim so callers don't have to special-case this.
        out.patched.assign(words, words + word_count);
        out.push_constant_offset_bytes = 0;
        out.push_constant_size_bytes   = 0;
        return true;
    }

    // For the GID path we need LocalInvocationId. If the module already
    // declares it, reuse; otherwise we'll declare a fresh Input variable.
    const bool synthesise_lid = need_gid && (md.lid_var_id == 0);

    //-------------------------------------------------------------------
    // Pre-scan: identify every load/access-chain site that needs redirect.
    //-------------------------------------------------------------------
    std::vector<RedirectSite> sites;

    // For OpAccessChain on WGID/GID var: result-id is a pointer; later
    // OpLoads on that pointer are the actual values. Track which pointer
    // result-ids derive from WGID/GID and the component index.
    struct DerivedPtr { RedirectSite::Source source; uint32_t component; };
    std::unordered_map<uint32_t, DerivedPtr> derived_uint_ptrs;

    // First pass: capture full-vector OpLoads + collect derived pointers.
    ForEachInstruction(words, word_count, [&](size_t pos, uint32_t op, uint32_t oc, const uint32_t* o) {
        if (op == spv::OpLoad && oc >= 3)
        {
            uint32_t result_type = o[0];
            uint32_t result_id   = o[1];
            uint32_t ptr         = o[2];
            if (result_type == md.uvec3_type_id)
            {
                if (need_wgid && ptr == md.wgid_var_id)
                {
                    sites.push_back({ pos, op, result_id, RedirectSite::Source::Wgid, RedirectSite::Form::FullLoad, 0 });
                }
                else if (need_gid && ptr == md.gid_var_id)
                {
                    sites.push_back({ pos, op, result_id, RedirectSite::Source::Gid, RedirectSite::Form::FullLoad, 0 });
                }
            }
        }
        else if ((op == spv::OpAccessChain || op == spv::OpInBoundsAccessChain) && oc >= 3)
        {
            // result_type = o[0]; result_id = o[1]; base = o[2]; indices = o[3..]
            uint32_t result_type = o[0];
            uint32_t result_id   = o[1];
            uint32_t base        = o[2];
            // We only care about chains rooted at WGID/GID with a single
            // constant uint index resolving to 0/1/2. That covers GID.x etc.
            bool match_wgid = need_wgid && base == md.wgid_var_id;
            bool match_gid  = need_gid  && base == md.gid_var_id;
            if (!match_wgid && !match_gid) return true;
            if (result_type != md.uint_ptr_input_id) return true;
            if (oc < 4) return true; // expect at least one index
            // Single index expected for accessing a vector component.
            if (oc != 4) return true;
            uint32_t idx_id = o[3];
            uint32_t comp   = 0xFFFFFFFFu;
            if (idx_id == md.const_uint_0_id) comp = 0;
            else if (idx_id == md.const_uint_1_id) comp = 1;
            else if (idx_id == md.const_uint_2_id) comp = 2;
            if (comp == 0xFFFFFFFFu) return true;
            DerivedPtr d;
            d.source    = match_wgid ? RedirectSite::Source::Wgid : RedirectSite::Source::Gid;
            d.component = comp;
            derived_uint_ptrs[result_id] = d;
        }
        return true;
    });

    // Second pass: capture OpLoads on derived uint pointers.
    ForEachInstruction(words, word_count, [&](size_t pos, uint32_t op, uint32_t oc, const uint32_t* o) {
        if (op == spv::OpLoad && oc >= 3 && o[0] == md.uint_type_id)
        {
            auto it = derived_uint_ptrs.find(o[2]);
            if (it != derived_uint_ptrs.end())
            {
                sites.push_back({ pos, op, o[1], it->second.source, RedirectSite::Form::ComponentLoad, it->second.component });
            }
        }
        return true;
    });

    //-------------------------------------------------------------------
    // Allocate new IDs. We need:
    //   - uint type if absent (id_uint)
    //   - uvec3 type if absent (id_uvec3)
    //   - pointer-to-uvec3 Input (id_ptr_uvec3_in) — only if synthesising LID
    //   - pointer-to-uint   Input (id_ptr_uint_in) — only if reading LID per-comp
    //   - const uint 0/1/2 (if not present)
    //   - struct { uvec3 } (synth PC struct)         id_pc_struct
    //   - ptr to struct, PushConstant                id_ptr_pc_struct
    //   - ptr to uvec3, PushConstant                 id_ptr_uvec3_pc
    //   - the push-constant variable                 id_pc_var
    //   - LID variable (only if synthesise_lid)      id_lid_var
    //   - synth_wgid_uvec3 SSA load                  id_synth_wgid
    //   - synth_wgid component extracts (x,y,z)      id_synth_wgid_x, _y, _z
    //   - synth_gid_uvec3 SSA value                  id_synth_gid (only if need_gid)
    //   - synth_gid component extracts (x,y,z)       id_synth_gid_x, _y, _z (need_gid)
    //   - per-site "dummy" id renaming for OpLoad/OpAccessChain (the original
    //     result-id is reused for the OpCopyObject we emit after).
    //
    // To minimise risk of stomping later instruction ID assumptions, we
    // assign NEW ids in [bound .. bound+N) and leave existing IDs alone.
    //-------------------------------------------------------------------
    uint32_t next_id = md.bound;
    auto alloc = [&]() { return next_id++; };

    uint32_t id_uint        = md.uint_type_id;
    uint32_t id_uvec3       = md.uvec3_type_id;
    uint32_t id_ptr_uint_in = md.uint_ptr_input_id;
    uint32_t id_ptr_uvec3_in= md.uvec3_ptr_input_id;
    uint32_t id_c0          = md.const_uint_0_id;
    uint32_t id_c1          = md.const_uint_1_id;
    uint32_t id_c2          = md.const_uint_2_id;
    if (id_uint == 0)        id_uint        = alloc();
    if (id_uvec3 == 0)       id_uvec3       = alloc();
    if (id_ptr_uint_in == 0) id_ptr_uint_in = alloc();
    if (id_ptr_uvec3_in == 0)id_ptr_uvec3_in= alloc();
    if (id_c0 == 0)          id_c0          = alloc();
    if (id_c1 == 0)          id_c1          = alloc();
    if (id_c2 == 0)          id_c2          = alloc();

    uint32_t id_pc_struct       = alloc();
    uint32_t id_ptr_pc_struct   = alloc();
    uint32_t id_ptr_uvec3_pc    = alloc();
    uint32_t id_pc_var          = alloc();

    uint32_t id_lid_var = md.lid_var_id;
    if (synthesise_lid) id_lid_var = alloc();

    uint32_t id_synth_pc_ptr   = alloc();
    uint32_t id_synth_wgid     = alloc();
    uint32_t id_synth_wgid_x   = alloc();
    uint32_t id_synth_wgid_y   = alloc();
    uint32_t id_synth_wgid_z   = alloc();

    uint32_t id_lid_full       = 0;
    uint32_t id_local_size_x   = 0;
    uint32_t id_local_size_y   = 0;
    uint32_t id_local_size_z   = 0;
    uint32_t id_local_size_vec = 0;
    uint32_t id_wgid_times_ls  = 0;
    uint32_t id_synth_gid      = 0;
    uint32_t id_synth_gid_x    = 0;
    uint32_t id_synth_gid_y    = 0;
    uint32_t id_synth_gid_z    = 0;
    if (need_gid)
    {
        id_lid_full       = alloc();
        id_local_size_x   = alloc();
        id_local_size_y   = alloc();
        id_local_size_z   = alloc();
        id_local_size_vec = alloc();
        id_wgid_times_ls  = alloc();
        id_synth_gid      = alloc();
        id_synth_gid_x    = alloc();
        id_synth_gid_y    = alloc();
        id_synth_gid_z    = alloc();
    }

    // Per-site dummy ids: the original instruction's result-id is reused as
    // the OpCopyObject's result-id (so all downstream consumers Just Work).
    // The original instruction is rewritten to produce a fresh, unused id.
    for (auto& s : sites)
    {
        // We don't store the dummy id on the site itself; we'll generate one
        // when emitting that instruction. To keep the IDs contiguous we
        // pre-allocate them here, in the order we'll need them.
    }
    std::unordered_map<size_t, uint32_t> site_dummy_id_by_pos;
    for (auto& s : sites)
    {
        site_dummy_id_by_pos[s.inst_pos] = alloc();
    }

    const uint32_t new_bound = next_id;

    //-------------------------------------------------------------------
    // Push-constant member offset. Modules in this capture use at most ~24
    // bytes of push-constants, so we align up to 16 and place our uvec3
    // there. uvec3 in std430 / scalar block layout occupies 12 bytes;
    // we declare a single member at offset = AlignUp(extent, 16).
    //-------------------------------------------------------------------
    const uint32_t synth_offset = AlignUp(md.existing_pc_extent_bytes, 16);
    const uint32_t synth_size   = 12;

    //-------------------------------------------------------------------
    // Emit the patched module.
    //-------------------------------------------------------------------
    out.patched.clear();
    out.patched.reserve(word_count + 256);
    // Header (5 words). Update bound.
    out.patched.push_back(words[0]);
    out.patched.push_back(words[1]);
    out.patched.push_back(words[2]);
    out.patched.push_back(new_bound);
    out.patched.push_back(words[4]);

    // We'll walk the original instructions in order and emit them, with a
    // handful of section-specific insertions and per-site rewrites.

    // Track section transitions. SPIR-V instructions appear in a defined
    // order: capabilities/extensions/memory model first, then entry points,
    // then execution modes, then debug, then annotations, then types/consts/
    // globals, then functions. We insert in the appropriate section.
    enum class Section { Pre, AfterEntryPoints, AfterAnnotations, Types, Functions, Done };
    Section section = Section::Pre;

    bool decorations_emitted = false;
    bool types_emitted       = false;

    // Build extra interface IDs for the entry point, if we'll declare new
    // Input variables (only LID if synthesised).
    std::vector<uint32_t> extra_iface;
    if (synthesise_lid) extra_iface.push_back(id_lid_var);

    // Pre-compute a set of instruction positions that we redirect, plus a
    // fast lookup from position -> RedirectSite.
    std::unordered_map<size_t, const RedirectSite*> site_by_pos;
    for (auto& s : sites) site_by_pos[s.inst_pos] = &s;

    auto emit_decoration_block = [&]() {
        // Decorate the push-constant struct as Block.
        EmitInst(out.patched, spv::OpDecorate, { id_pc_struct, spv::DecorationBlock });
        // MemberDecorate offset.
        EmitInst(out.patched, spv::OpMemberDecorate, { id_pc_struct, 0, spv::DecorationOffset, synth_offset });
        if (synthesise_lid)
        {
            EmitInst(out.patched, spv::OpDecorate, { id_lid_var, spv::DecorationBuiltIn, spv::BuiltInLocalInvocationId });
        }
    };

    auto emit_new_types_and_vars = [&]() {
        // Emit only the types/consts/vars that we allocated fresh.
        if (id_uint != md.uint_type_id)
        {
            EmitInst(out.patched, spv::OpTypeInt, { id_uint, 32, 0 });
        }
        if (id_uvec3 != md.uvec3_type_id)
        {
            EmitInst(out.patched, spv::OpTypeVector, { id_uvec3, id_uint, 3 });
        }
        if (id_ptr_uint_in != md.uint_ptr_input_id)
        {
            EmitInst(out.patched, spv::OpTypePointer, { id_ptr_uint_in, spv::StorageClassInput, id_uint });
        }
        if (id_ptr_uvec3_in != md.uvec3_ptr_input_id)
        {
            EmitInst(out.patched, spv::OpTypePointer, { id_ptr_uvec3_in, spv::StorageClassInput, id_uvec3 });
        }
        if (id_c0 != md.const_uint_0_id)
        {
            EmitInst(out.patched, spv::OpConstant, { id_uint, id_c0, 0 });
        }
        if (id_c1 != md.const_uint_1_id)
        {
            EmitInst(out.patched, spv::OpConstant, { id_uint, id_c1, 1 });
        }
        if (id_c2 != md.const_uint_2_id)
        {
            EmitInst(out.patched, spv::OpConstant, { id_uint, id_c2, 2 });
        }
        // Synth push-constant struct: struct { uvec3 synth_wgid; }
        EmitInst(out.patched, spv::OpTypeStruct, { id_pc_struct, id_uvec3 });
        EmitInst(out.patched, spv::OpTypePointer, { id_ptr_pc_struct, spv::StorageClassPushConstant, id_pc_struct });
        EmitInst(out.patched, spv::OpTypePointer, { id_ptr_uvec3_pc, spv::StorageClassPushConstant, id_uvec3 });
        // Push-constant variable.
        EmitInst(out.patched, spv::OpVariable, { id_ptr_pc_struct, id_pc_var, spv::StorageClassPushConstant });
        if (synthesise_lid)
        {
            EmitInst(out.patched, spv::OpVariable, { id_ptr_uvec3_in, id_lid_var, spv::StorageClassInput });
        }
        // LocalSize constants + composite for GID synthesis (constants must
        // live in the global section, not inside any function body).
        if (need_gid)
        {
            EmitInst(out.patched, spv::OpConstant, { id_uint, id_local_size_x, md.local_size[0] });
            EmitInst(out.patched, spv::OpConstant, { id_uint, id_local_size_y, md.local_size[1] });
            EmitInst(out.patched, spv::OpConstant, { id_uint, id_local_size_z, md.local_size[2] });
            EmitInst(out.patched, spv::OpConstantComposite,
                     { id_uvec3, id_local_size_vec, id_local_size_x, id_local_size_y, id_local_size_z });
        }
    };

    // Walk instructions.
    size_t pos = kSpirvHeaderWords;
    bool   first_label_in_entry_emitted = false;
    bool   in_entry_func    = false;

    while (pos < word_count)
    {
        uint32_t inst_word = words[pos];
        uint32_t wc        = inst_word >> 16;
        uint32_t op        = inst_word & 0xFFFFu;
        if (wc == 0 || pos + wc > word_count) break;
        const uint32_t  oc = wc - 1;
        const uint32_t* o  = words + pos + 1;

        // Section bookkeeping: emit our extra annotations before the first
        // function and after the last annotation. We use the first "type"
        // instruction as the marker for inserting types/vars; we use the
        // first OpFunction as the marker for emitting any annotations we
        // haven't yet emitted (defensive: in case the module has no
        // annotations at all).

        auto is_type_const_var = [](uint32_t opv) {
            switch (opv)
            {
            case spv::OpTypeVoid: case spv::OpTypeBool: case spv::OpTypeInt:
            case spv::OpTypeFloat: case spv::OpTypeVector: case spv::OpTypeMatrix:
            case spv::OpTypeImage: case spv::OpTypeSampler: case spv::OpTypeSampledImage:
            case spv::OpTypeArray: case spv::OpTypeRuntimeArray: case spv::OpTypeStruct:
            case spv::OpTypeOpaque: case spv::OpTypePointer: case spv::OpTypeFunction:
            case spv::OpTypeEvent: case spv::OpTypeDeviceEvent: case spv::OpTypeReserveId:
            case spv::OpTypeQueue: case spv::OpTypePipe: case spv::OpTypeForwardPointer:
            case spv::OpTypeAccelerationStructureKHR:
            case spv::OpConstant: case spv::OpConstantTrue: case spv::OpConstantFalse:
            case spv::OpConstantComposite: case spv::OpConstantNull:
            case spv::OpSpecConstant: case spv::OpSpecConstantTrue:
            case spv::OpSpecConstantFalse: case spv::OpSpecConstantComposite:
            case spv::OpSpecConstantOp:
            case spv::OpVariable:
                return true;
            default:
                return false;
            }
        };
        auto is_annotation = [](uint32_t opv) {
            switch (opv)
            {
            case spv::OpDecorate: case spv::OpMemberDecorate:
            case spv::OpGroupDecorate: case spv::OpGroupMemberDecorate:
            case spv::OpDecorationGroup: case spv::OpDecorateId:
            case spv::OpDecorateString: case spv::OpMemberDecorateString:
                return true;
            default:
                return false;
            }
        };

        // Insert our new decorations after the existing annotation section.
        if (!decorations_emitted && (is_type_const_var(op) || op == spv::OpFunction))
        {
            emit_decoration_block();
            decorations_emitted = true;
        }
        // Insert new types/vars before functions start.
        if (!types_emitted && op == spv::OpFunction)
        {
            emit_new_types_and_vars();
            types_emitted = true;
        }

        // OpEntryPoint: extend interface list if we added new Input vars.
        if (op == spv::OpEntryPoint)
        {
            if (oc >= 2 && o[0] == spv::ExecutionModelGLCompute && !extra_iface.empty())
            {
                WriteEntryPointPossiblyExtended(out.patched, words, md, extra_iface);
                pos += wc;
                continue;
            }
            // Otherwise pass through unchanged.
        }

        // OpFunction: enter function. Track whether it's the entry function
        // so we can inject synth-value computation in its first block.
        if (op == spv::OpFunction)
        {
            if (oc >= 2)
            {
                in_entry_func = (o[1] == md.entry_point_id);
            }
            first_label_in_entry_emitted = false;
        }
        if (op == spv::OpFunctionEnd)
        {
            in_entry_func = false;
        }

        // Copy current instruction (possibly rewriting it for redirect sites).
        const RedirectSite* site = nullptr;
        auto sit = site_by_pos.find(pos);
        if (sit != site_by_pos.end()) site = sit->second;

        if (site != nullptr)
        {
            // Emit the original instruction with the result_id replaced by a
            // fresh dummy ID. SPIR-V layout: for OpLoad and OpAccessChain
            // (both have result_type at operand 0 and result_id at operand 1).
            uint32_t dummy_id = site_dummy_id_by_pos[pos];
            std::vector<uint32_t> ops(o, o + oc);
            ops[1] = dummy_id;
            EmitInstWithOperands(out.patched, op, ops.data(), oc);
            // Then emit the OpCopyObject that produces the original result-id
            // bound to the synthetic value of the appropriate kind.
            uint32_t result_type = ops[0];
            uint32_t src_value   = 0;
            if (site->form == RedirectSite::Form::FullLoad)
            {
                src_value = (site->source == RedirectSite::Source::Wgid) ? id_synth_wgid : id_synth_gid;
            }
            else // ComponentLoad
            {
                uint32_t arr[3] = {
                    site->source == RedirectSite::Source::Wgid ? id_synth_wgid_x : id_synth_gid_x,
                    site->source == RedirectSite::Source::Wgid ? id_synth_wgid_y : id_synth_gid_y,
                    site->source == RedirectSite::Source::Wgid ? id_synth_wgid_z : id_synth_gid_z,
                };
                src_value = arr[site->component_index];
            }
            EmitInst(out.patched, spv::OpCopyObject, { result_type, site->orig_result_id, src_value });
        }
        else
        {
            EmitInstWithOperands(out.patched, op, o, oc);
        }

        // If this is the first OpLabel inside the entry function, inject
        // our synth-value computation right after it.
        if (op == spv::OpLabel && in_entry_func && !first_label_in_entry_emitted)
        {
            first_label_in_entry_emitted = true;

            // %synth_pc_ptr = OpAccessChain ptr_uvec3_PushConstant pc_var c0
            EmitInst(out.patched, spv::OpAccessChain, { id_ptr_uvec3_pc, id_synth_pc_ptr, id_pc_var, id_c0 });
            // %synth_wgid = OpLoad uvec3 %synth_pc_ptr
            EmitInst(out.patched, spv::OpLoad, { id_uvec3, id_synth_wgid, id_synth_pc_ptr });
            // Component extracts.
            EmitInst(out.patched, spv::OpCompositeExtract, { id_uint, id_synth_wgid_x, id_synth_wgid, 0 });
            EmitInst(out.patched, spv::OpCompositeExtract, { id_uint, id_synth_wgid_y, id_synth_wgid, 1 });
            EmitInst(out.patched, spv::OpCompositeExtract, { id_uint, id_synth_wgid_z, id_synth_wgid, 2 });

            if (need_gid)
            {
                // %lid_full = OpLoad uvec3 %lid_var
                EmitInst(out.patched, spv::OpLoad, { id_uvec3, id_lid_full, id_lid_var });
                // %wgid_times_ls = %synth_wgid * local_size_vec
                EmitInst(out.patched, spv::OpIMul, { id_uvec3, id_wgid_times_ls, id_synth_wgid, id_local_size_vec });
                // %synth_gid = wgid_times_ls + lid_full
                EmitInst(out.patched, spv::OpIAdd, { id_uvec3, id_synth_gid, id_wgid_times_ls, id_lid_full });
                EmitInst(out.patched, spv::OpCompositeExtract, { id_uint, id_synth_gid_x, id_synth_gid, 0 });
                EmitInst(out.patched, spv::OpCompositeExtract, { id_uint, id_synth_gid_y, id_synth_gid, 1 });
                EmitInst(out.patched, spv::OpCompositeExtract, { id_uint, id_synth_gid_z, id_synth_gid, 2 });
            }
        }

        pos += wc;
    }

    // Safety: if for some reason we never saw OpFunction, our types_emitted
    // / decorations_emitted may still be false. Emit them now (they belong
    // at the end of the global section, which is the end of the module).
    if (!decorations_emitted) emit_decoration_block();
    if (!types_emitted)       emit_new_types_and_vars();

    out.push_constant_offset_bytes = synth_offset;
    out.push_constant_size_bytes   = synth_size;
    return true;
}

GFXRECON_END_NAMESPACE(util)
GFXRECON_END_NAMESPACE(gfxrecon)
