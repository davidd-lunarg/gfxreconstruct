/*
** Copyright (c) 2022 LunarG, Inc.
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

#ifndef GFXRECON_DX12_RESOURCE_VALUE_TRACKING_CONSUMER_H
#define GFXRECON_DX12_RESOURCE_VALUE_TRACKING_CONSUMER_H

#include "generated/generated_dx12_replay_consumer.h"
#include "util/defines.h"

#include <map>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)

// Although this class lives in the optimize tool project, it is derived from decode::Dx12ReplayConsumer so put it in
// the decode namespace.
GFXRECON_BEGIN_NAMESPACE(decode)

class Dx12ResourceValueTrackingConsumer : public Dx12ReplayConsumer
{
  public:
    Dx12ResourceValueTrackingConsumer(std::shared_ptr<application::Application> application,
                                      const DxReplayOptions&                    options,
                                      bool                                      experimental_tracker);

    virtual ~Dx12ResourceValueTrackingConsumer() override {}

    void GetTrackedResourceValues(Dx12FillCommandResourceValueMap& values);

    void GetUnassociatedResourceValues(Dx12UnassociatedResourceValueMap& unassociated_values);

    void SetUnassociatedResourceValues(Dx12FillCommandResourceValueMap&&  tracked_values,
                                       Dx12UnassociatedResourceValueMap&& unassociated_values);

    void GetResourceValueAuditSummary(Dx12ResourceValueAuditSummary& summary);

    // Second-pass exports for the perturbation (verification) pass.
    void GetResourceValueScanHits(std::vector<Dx12ScanHitCandidate>& hits);
    void GetNeedleAllocations(std::map<format::HandleId, Dx12CaptureAllocation>& allocations);
    void GetUnresolvedGpuVaValues(std::unordered_map<uint64_t, format::HandleId>& values);

    // Argument/count buffer byte ranges consumed by executed (non-RV) ExecuteIndirect calls, keyed by
    // resource. Candidates in these ranges must not be tagged: a perturbed count or argument executes.
    typedef std::map<format::HandleId, std::set<std::pair<uint64_t, uint64_t>>> NonRvExecuteIndirectRanges;
    const NonRvExecuteIndirectRanges& GetNonRvExecuteIndirectRanges() const { return non_rv_ei_ranges_; }

    // Enter the verification pass: tagged bytes are patched into fill/init payloads before normal
    // processing, and the mapper decodes use-site observations against the plan.
    typedef std::unordered_map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>> PerturbationPatchMap;
    void SetResourceValuePerturbation(PerturbationPatchMap&& patches, Dx12PerturbationPlan&& plan);
    void GetPerturbationResults(Dx12PerturbationResults& results);

    virtual void
    ProcessFillMemoryCommand(uint64_t memory_id, uint64_t offset, uint64_t size, const uint8_t* data) override;

    virtual void ProcessInitSubresourceCommand(const format::InitSubresourceCommandHeader& command_header,
                                               const uint8_t*                              data) override;

    virtual void Process_ID3D12GraphicsCommandList4_CopyRaytracingAccelerationStructure(
        const ApiCallInfo&                                call_info,
        format::HandleId                                  object_id,
        D3D12_GPU_VIRTUAL_ADDRESS                         DestAccelerationStructureData,
        D3D12_GPU_VIRTUAL_ADDRESS                         SourceAccelerationStructureData,
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE Mode) override;

    virtual void ProcessInitDx12AccelerationStructureCommand(
        const format::InitDx12AccelerationStructureCommandHeader&                           command_header,
        const std::vector<format::InitDx12AccelerationStructureGeometryDesc>&               geometry_descs,
        StructPointerDecoder<Decoded_D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS>* build_inputs,
        const uint8_t*                                                                      build_inputs_data) override;

    virtual void OverrideExecuteIndirect(DxObjectInfo* command_list_object_info,
                                         DxObjectInfo* command_signature_object_info,
                                         UINT          max_command_count,
                                         DxObjectInfo* argument_buffer_object_info,
                                         UINT64        argument_buffer_offset,
                                         DxObjectInfo* count_buffer_object_info,
                                         UINT64        count_buffer_offset) override;

    virtual void OverrideBuildRaytracingAccelerationStructure(
        DxObjectInfo*                                                                     command_list4_object_info,
        StructPointerDecoder<Decoded_D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC>* desc,
        UINT                                                                              num_post_build_info_descs,
        StructPointerDecoder<Decoded_D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC>*
            post_build_info_descs) override;

    virtual void OverrideDispatchRays(DxObjectInfo*                                           command_list4_object_info,
                                      StructPointerDecoder<Decoded_D3D12_DISPATCH_RAYS_DESC>* desc_decoder) override;

    // Enable/disable replay of calls the depend on having correclty mapped resource values (DXR and EI calls). Default
    // is that these calls are enabled.
    void EnableReplayOfResourceValueCalls(bool enable) { replay_resource_value_calls_ = enable; }

  private:
    // Returns data, or a patched copy of it when perturbation entries exist for the current block.
    // base_offset is the payload's offset within the annotation space (the fill's memory offset; 0 for
    // init subresource payloads).
    const uint8_t* ApplyResourceValuePerturbation(uint64_t base_offset, uint64_t size, const uint8_t* data);

  private:
    bool replay_resource_value_calls_;

    PerturbationPatchMap       perturbation_patches_;
    std::vector<uint8_t>       perturbation_buffer_;
    NonRvExecuteIndirectRanges non_rv_ei_ranges_;
};

GFXRECON_END_NAMESPACE(decode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_DX12_RESOURCE_VALUE_TRACKING_CONSUMER_H
