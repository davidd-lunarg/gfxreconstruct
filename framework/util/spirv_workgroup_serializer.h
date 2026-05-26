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

#ifndef GFXRECONSTRUCT_UTIL_SPIRV_WORKGROUP_SERIALIZER_H
#define GFXRECONSTRUCT_UTIL_SPIRV_WORKGROUP_SERIALIZER_H

#include "util/defines.h"

#include <cstddef>
#include <cstdint>
#include <vector>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(util)

// Returns true if the SPIR-V module contains any OpAtomic* instruction.
// `words` points to the start of the module (including the 5-word header);
// `word_count` is the number of 32-bit words.
bool ContainsAtomicOps(const uint32_t* words, size_t word_count);

/*
 * Output of PatchForWorkgroupSerialization. On success, `patched` holds the
 * rewritten SPIR-V module, and `push_constant_offset_bytes` / `push_constant_size_bytes`
 * describe the byte range in the (possibly newly-introduced) push-constant
 * block that the caller must write with the synthetic gl_WorkGroupID before
 * each (1,1,1) dispatch. The range is exactly 12 bytes (uvec3) and is
 * placed at a 16-byte-aligned offset that does not overlap any push-constant
 * range the original module already used.
 */
struct WorkgroupSerializerOutput
{
    std::vector<uint32_t> patched;
    uint32_t              push_constant_offset_bytes = 0;
    uint32_t              push_constant_size_bytes   = 0;
};

/*
 * Rewrite a compute SPIR-V module so that every read of BuiltIn WorkgroupId
 * or GlobalInvocationId returns a synthetic value, fed via push constant,
 * instead of the value supplied by the driver. The atomic ops themselves
 * are untouched; the caller arranges to dispatch the pipeline as gx*gy*gz
 * separate (1,1,1) dispatches, pushing a synthetic (x,y,z) value before
 * each.
 *
 * Returns false if:
 *   - `words` is not a valid SPIR-V module,
 *   - the module is not a compute shader (no GLCompute entry point),
 *   - LocalSize is spec-constant (OpExecutionModeId LocalSizeId — punted in
 *     v1; spec-constant LocalSize would also require synthesising
 *     LocalSizeId-aware multiplications),
 *   - LocalInvocationId is used by reads of GlobalInvocationId but the
 *     module does not declare a LocalInvocationId Input variable (we
 *     synthesise a fresh declaration in that case, but only if the rest of
 *     the prerequisites are met).
 *
 * On failure the contents of `out` are undefined; the caller must fall back
 * to the non-serialised dispatch path for this module.
 */
bool PatchForWorkgroupSerialization(const uint32_t*            words,
                                    size_t                     word_count,
                                    WorkgroupSerializerOutput& out);

GFXRECON_END_NAMESPACE(util)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECONSTRUCT_UTIL_SPIRV_WORKGROUP_SERIALIZER_H
