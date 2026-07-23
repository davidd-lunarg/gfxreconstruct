/*
** Copyright (c) 2022 LunarG, Inc.
** Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
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
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#ifndef GFXRECON_DECODE_DX12_RESOURCE_VALUE_TRACKER_H
#define GFXRECON_DECODE_DX12_RESOURCE_VALUE_TRACKER_H

#include "decode/dx12_object_info.h"
#include "decode/handle_pointer_decoder.h"
#include "decode/struct_pointer_decoder.h"
#include "graphics/dx12_gpu_va_map.h"
#include "util/defines.h"

#include <unordered_map>
#include <unordered_set>
#include <utility>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(decode)

typedef uint64_t Dx12FillCommandBlockIndex;
typedef uint64_t Dx12FillCommandByteOffset;
#pragma pack(push)
#pragma pack(1)
// There will be many Dx12FillCommandResourceValues. Set struct packing to 1 to minimize memory used.
struct Dx12FillCommandResourceValue
{
    Dx12FillCommandByteOffset offset;
    ResourceValueType         type;
};
#pragma pack(pop)
typedef std::map<Dx12FillCommandBlockIndex, std::vector<Dx12FillCommandResourceValue>> Dx12FillCommandResourceValueMap;

#pragma pack(push)
#pragma pack(1)
struct Dx12UnassociatedResourceValue
{
  private:
    static constexpr size_t kMaxResourceValueSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;

  public:
    ResourceValueType type{ ResourceValueType::kUnknown };

    union
    {
        graphics::Dx12ShaderIdentifier shader_id;
        format::HandleId               resource_id{ format::kNullHandleId };
    };
};
#pragma pack(pop)
struct Dx12UnassociatedResourceValueGroup
{
    uint64_t                                   block_index;
    std::vector<Dx12UnassociatedResourceValue> values;
};
typedef std::unordered_map<format::HandleId, std::vector<Dx12UnassociatedResourceValueGroup>>
    Dx12UnassociatedResourceValueMap;

// Closure-ledger counts for one resource value type, from the audit pass. Each observed use-site value is
// classified into exactly one of walk/candidate/dead/unresolved; post_exec_* are independent observations
// of the same values.
struct Dx12ResourceValueAuditCounts
{
    uint64_t total{ 0 };                ///< Use-site values observed by the mapper.
    uint64_t satisfied_by_walk{ 0 };    ///< Attributed to a fill/init block by the offset walk.
    uint64_t covered_by_candidate{ 0 }; ///< Not walk-attributed, but the exact value was found by the content scan.
    uint64_t dead_capture_va{ 0 };      ///< Capture address falls in no tracked allocation; can never be mapped.
    uint64_t unresolved{ 0 };           ///< No walk attribution and no content-scan coverage.
    uint64_t post_exec_checked{ 0 };    ///< Values compared between pre- and post-execution readbacks.
    uint64_t post_exec_changed{ 0 };    ///< Values whose bytes changed during the submission that consumed them.
};

// One distinct unresolved GPU VA, recorded the first time it is observed.
struct Dx12UnresolvedValueSample
{
    uint64_t         value{ 0 };
    format::HandleId target_resource_id{ format::kNullHandleId }; ///< Resource the VA points into.
    uint64_t         first_seen_block{ 0 };
};

struct Dx12ResourceValueAuditSummary
{
    Dx12ResourceValueAuditCounts gpu_va;
    Dx12ResourceValueAuditCounts descriptor_handle;
    Dx12ResourceValueAuditCounts shader_id;

    uint64_t distinct_unresolved_gpu_vas{ 0 };
    uint64_t distinct_dead_capture_vas{ 0 };
    uint64_t distinct_unresolved_shader_ids{ 0 };

    // Unresolved observation counts keyed by the resource the value was read from.
    std::map<format::HandleId, uint64_t> unresolved_by_resource;

    // Unresolved GPU VA observation counts keyed by the resource the value points into.
    std::map<format::HandleId, uint64_t> unresolved_by_target_resource;

    // First-seen samples of distinct unresolved GPU VAs (bounded; for diagnosing the residue).
    std::vector<Dx12UnresolvedValueSample> unresolved_gpu_va_samples;
};

// One content-scan hit from the second experimental pass. offset is in annotation space: the same space as
// Dx12FillCommandResourceValue::offset for the hit's fill or init block.
struct Dx12ScanHitCandidate
{
    uint64_t          block_index{ 0 };
    uint64_t          offset{ 0 };
    ResourceValueType type{ ResourceValueType::kUnknown };
    uint64_t          value{ 0 }; ///< The hit's 8-byte window as a little-endian u64; 0 for shader IDs.
    format::HandleId  target_resource_id{ format::kNullHandleId };   ///< VA hits: resource the value points into.
    format::HandleId  location_resource_id{ format::kNullHandleId }; ///< Resource whose payload holds the hit.
};

// Capture-space allocation bounds of a needle resource (a resource some unassociated value points into).
struct Dx12CaptureAllocation
{
    uint64_t capture_address{ 0 };
    uint64_t width{ 0 };
};

// Perturbation plan for the verification pass: candidate GPU VA values tagged with in-allocation deltas,
// plus every observation those tags can legally produce at a use site. Decode is by exact value match, so
// all keys are kept globally collision-free at build time.
struct Dx12DerivedExpectation
{
    uint64_t unresolved_value{ 0 }; ///< The GPU-derived value whose chain this expectation confirms.
    uint64_t base_value{ 0 };       ///< The tagged candidate value identified as the chain's base.
};

struct Dx12PerturbationPlan
{
    std::unordered_map<uint64_t, uint64_t>               value_to_tagged; ///< candidate original -> tagged
    std::unordered_map<uint64_t, uint64_t>               tagged_to_value; ///< tagged -> candidate original
    std::unordered_map<uint64_t, Dx12DerivedExpectation> derived_expectations; ///< keyed by expected observation
    std::unordered_set<uint64_t>                         untested_values; ///< no collision-free in-range delta
    std::unordered_set<uint64_t>                         unresolved_values;
    uint64_t                                             ambiguous_expectations{ 0 };
};

struct Dx12PerturbationResults
{
    uint64_t copied_decodes{ 0 };        ///< Observations equal to a tagged candidate value.
    uint64_t derived_decodes{ 0 };       ///< Observations equal to an expected derived value.
    uint64_t untagged_candidate_observations{ 0 }; ///< Observations equal to a raw (untagged flow) candidate.
    uint64_t untested_observations{ 0 };
    uint64_t unverified_unresolved_observations{ 0 };

    std::unordered_set<uint64_t> confirmed_values;         ///< Candidate values proven to reach a use site.
    std::unordered_set<uint64_t> derived_confirmed_values; ///< Unresolved values proven derived from a tagged base.
    std::unordered_set<uint64_t> observed_untagged_values;
    std::unordered_set<uint64_t> unverified_unresolved_values;
};

class Dx12ResourceValueTracker
{
  public:
    Dx12ResourceValueTracker(std::function<DxObjectInfo*(format::HandleId id)> get_object_info_func,
                             std::function<uint64_t(void)>                     get_current_block_index_func) :
        get_object_info_func_(get_object_info_func),
        get_current_block_index_func_(get_current_block_index_func)
    {}

    // When Dx12ResourceValueMapper encounters a resource value that needs to be mapped, it will call
    // AddTrackedResourceValue which tries to determine which FillMemoryCommand originally wrote the data based on the
    // FillMemoryCommands and resource copies previously processed by the Dx12ResourceValueTracker.
    virtual bool AddTrackedResourceValue(format::HandleId              resource_id,
                                         ResourceValueType             type,
                                         uint64_t                      offset,
                                         const uint8_t*                resource_value_data,
                                         const graphics::Dx12GpuVaMap& gpu_va_map);

    virtual void PostProcessExecuteCommandLists(DxObjectInfo*                             command_queue_object_info,
                                                UINT                                      num_command_lists,
                                                HandlePointerDecoder<ID3D12CommandList*>* command_lists_decoder);

    virtual void
    PostProcessFillMemoryCommand(uint64_t resource_id, uint64_t offset, uint64_t size, const uint8_t* data);

    virtual void PostProcessInitSubresourceCommand(ID3D12Resource*                             resource,
                                                   const format::InitSubresourceCommandHeader& command_header,
                                                   const uint8_t*                              data);

    virtual void GetTrackedResourceValues(Dx12FillCommandResourceValueMap& values);

  public:
    // Public so unit tests can construct fill-command state without a D3D12 device.
    typedef std::vector<std::pair<uint64_t, uint64_t>> ResourceRanges;

    struct TrackedFillCommandInfo
    {
        uint64_t fill_command_block_index{ 0 };
        uint64_t original_offset{ 0 }; ///< Offset into the original resource from the original fill command.
        uint64_t offset{ 0 }; ///< Offset into the containing resource that has data from the original fill command.
        uint64_t size{ 0 };   ///< How much data in the containing resource came from this fill command.
        std::shared_ptr<std::vector<uint8_t>>
            init_subresource_data; ///< A copy of the data from init subresource calls.
    };

  protected:

    struct ProcessExecuteCommandListArgs
    {
        std::vector<ResourceCopyInfo>              resource_copies;
        std::map<format::HandleId, ResourceRanges> non_dxr_resources;
    };

    void AddBlockResourceValue(uint64_t fill_command_block_index, uint64_t offset, ResourceValueType type);

    void UpdateFillCommandState(format::HandleId resource_id, const TrackedFillCommandInfo& new_fill_command);

    virtual void ProcessExecuteCommandList(ProcessExecuteCommandListArgs args);

    QueueSyncEventInfo CreateProcessExecuteCommandListSyncEvent(ProcessExecuteCommandListArgs args)
    {
        return QueueSyncEventInfo{ false, false, nullptr, 0, [this, captured_args = std::move(args)]() {
                                      ProcessExecuteCommandList(captured_args);
                                  } };
    }

    std::map<format::HandleId, std::map<uint64_t, TrackedFillCommandInfo>> tracked_fill_commands_;

    Dx12FillCommandResourceValueMap tracked_resource_values_;

    std::function<DxObjectInfo*(format::HandleId id)> get_object_info_func_;
    std::function<uint64_t(void)>                     get_current_block_index_func_;

  public:
    ////// Begin members to support experimental tracking for experimental DXR optimization:

    // After the first pass of experimental tracking, get the resource values that remained unassociated with a fill
    // memory or init subresource command.
    virtual void GetUnassociatedResourceValues(Dx12UnassociatedResourceValueMap& values) {}

    // The second pass attempts to resolve the unassociated values from the first pass. Set them here to indicate this
    // is a second pass.
    virtual void SetUnassociatedResourceValues(Dx12FillCommandResourceValueMap&&          tracked_values,
                                               decode::Dx12UnassociatedResourceValueMap&& unassociated_values)
    {}

    // Report-only closure ledger from the audit pass; the mapper merges its post-execution readback counts
    // into the same summary.
    virtual void GetAuditSummary(Dx12ResourceValueAuditSummary& summary) {}

    // Second-pass exports consumed by the perturbation (verification) pass.
    virtual void GetScanHitCandidates(std::vector<Dx12ScanHitCandidate>& hits) {}
    virtual void GetNeedleAllocations(std::map<format::HandleId, Dx12CaptureAllocation>& allocations) {}
    virtual void GetUnresolvedGpuVaValues(std::unordered_map<uint64_t, format::HandleId>& values) {}

    // The third (verification) pass: enter decode mode with a perturbation plan; each use-site GPU VA
    // observation is classified against the plan's expected tagged and derived values.
    virtual void SetPerturbationDecode(Dx12PerturbationPlan&& plan) {}
    virtual void GetPerturbationResults(Dx12PerturbationResults& results) {}

    virtual void AddShaderRecordData(format::HandleId                 resource_id,
                                     uint64_t                         offset,
                                     uint64_t                         shader_record_size,
                                     const uint8_t*                   shader_record_data,
                                     const graphics::Dx12GpuVaMap&    gpu_va_map,
                                     const decode::Dx12DescriptorMap& descriptor_map)
    {}

    virtual void PostProcessCopyTextureRegion(DxObjectInfo* command_list_object_info,
                                              StructPointerDecoder<Decoded_D3D12_TEXTURE_COPY_LOCATION>* dst_decoder,
                                              UINT                                                       dst_x,
                                              UINT                                                       dst_y,
                                              UINT                                                       dst_z,
                                              StructPointerDecoder<Decoded_D3D12_TEXTURE_COPY_LOCATION>* src_decoder,
                                              StructPointerDecoder<Decoded_D3D12_BOX>* src_box_decoder)
    {}

    virtual void PostProcessIASetIndexBuffer(DxObjectInfo* command_list_object_info,
                                             StructPointerDecoder<Decoded_D3D12_INDEX_BUFFER_VIEW>* views_decoder,
                                             const graphics::Dx12GpuVaMap&                          reverse_gpu_va_map)
    {}

    virtual void PostProcessIASetVertexBuffers(DxObjectInfo* command_list_object_info,
                                               UINT          start_slot,
                                               UINT          num_views,
                                               StructPointerDecoder<Decoded_D3D12_VERTEX_BUFFER_VIEW>* views_decoder,
                                               const graphics::Dx12GpuVaMap& reverse_gpu_va_map)
    {}

    virtual void PostProcessCommandListReset(DxObjectInfo* command_list_object_info) {}

    virtual void AddResourceGpuVa(format::HandleId          resource_id,
                                  D3D12_GPU_VIRTUAL_ADDRESS replay_address,
                                  UINT64                    width,
                                  D3D12_GPU_VIRTUAL_ADDRESS capture_address)
    {}

    virtual void RemoveResourceGpuVa(format::HandleId resource_id, uint64_t capture_address) {}

    virtual void AddGpuDescriptorHeap(const D3D12_GPU_DESCRIPTOR_HANDLE&     capture_gpu_start,
                                      const D3D12_GPU_DESCRIPTOR_HANDLE&     replay_gpu_start,
                                      D3D12_DESCRIPTOR_HEAP_TYPE             heap_info_descriptor_type,
                                      uint32_t                               heap_info_descriptor_count,
                                      std::shared_ptr<DescriptorIncrements>& heap_info_capture_increments,
                                      std::shared_ptr<DescriptorIncrements>& heap_info_replay_increments)
    {}

    virtual void RemoveGpuDescriptorHeap(uint64_t capture_address) {}
};

GFXRECON_END_NAMESPACE(decode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_DECODE_DX12_RESOURCE_VALUE_TRACKER_H
