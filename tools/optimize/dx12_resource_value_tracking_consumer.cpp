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

#include "dx12_resource_value_tracking_consumer.h"

#include "util/platform.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(decode)

Dx12ResourceValueTrackingConsumer::Dx12ResourceValueTrackingConsumer(
    std::shared_ptr<application::Application> application, const DxReplayOptions& options, bool experimental_tracker) :
    Dx12ReplayConsumer(application, options),
    replay_resource_value_calls_(true)
{
    auto get_current_block_index_func = std::bind(static_cast<uint64_t (Dx12ReplayConsumerBase::*)(void)>(
                                                      &Dx12ResourceValueTrackingConsumer::GetCurrentBlockIndex),
                                                  this);
    GetResourceValueMapper()->EnableResourceValueTracker(get_current_block_index_func, experimental_tracker);
}

void Dx12ResourceValueTrackingConsumer::GetTrackedResourceValues(Dx12FillCommandResourceValueMap& tracked_values)
{
    // For already DXR-optimized file, Dx12ResourceValueMapper is nullptr
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->GetTrackedResourceValues(tracked_values);
    }
}

void Dx12ResourceValueTrackingConsumer::GetUnassociatedResourceValues(
    Dx12UnassociatedResourceValueMap& unassociated_values)
{
    // For already DXR-optimized file, Dx12ResourceValueMapper is nullptr
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->GetUnassociatedResourceValues(unassociated_values);
    }
}

void Dx12ResourceValueTrackingConsumer::SetUnassociatedResourceValues(
    Dx12FillCommandResourceValueMap&& tracked_values, Dx12UnassociatedResourceValueMap&& unassociated_values)
{
    // For already DXR-optimized file, Dx12ResourceValueMapper is nullptr
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->SetUnassociatedResourceValues(std::move(tracked_values),
                                                                std::move(unassociated_values));
    }
}

void Dx12ResourceValueTrackingConsumer::GetResourceValueAuditSummary(Dx12ResourceValueAuditSummary& summary)
{
    // For already DXR-optimized file, Dx12ResourceValueMapper is nullptr
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->GetAuditSummary(summary);
    }
}

void Dx12ResourceValueTrackingConsumer::GetResourceValueScanHits(std::vector<Dx12ScanHitCandidate>& hits)
{
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->GetScanHitCandidates(hits);
    }
}

void Dx12ResourceValueTrackingConsumer::GetNeedleAllocations(
    std::map<format::HandleId, Dx12CaptureAllocation>& allocations)
{
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->GetNeedleAllocations(allocations);
    }
}

void Dx12ResourceValueTrackingConsumer::GetUnresolvedGpuVaValues(
    std::unordered_map<uint64_t, format::HandleId>& values)
{
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->GetUnresolvedGpuVaValues(values);
    }
}

void Dx12ResourceValueTrackingConsumer::SetResourceValuePerturbation(PerturbationPatchMap&&  patches,
                                                                     Dx12PerturbationPlan&& plan)
{
    if (GetResourceValueMapper() != nullptr)
    {
        perturbation_patches_ = std::move(patches);
        GetResourceValueMapper()->SetPerturbationDecode(std::move(plan));
    }
}

void Dx12ResourceValueTrackingConsumer::GetPerturbationResults(Dx12PerturbationResults& results)
{
    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->GetPerturbationResults(results);
    }
}

const uint8_t*
Dx12ResourceValueTrackingConsumer::ApplyResourceValuePerturbation(uint64_t base_offset, uint64_t size, const uint8_t* data)
{
    if (perturbation_patches_.empty() || (data == nullptr))
    {
        return data;
    }

    auto patches_iter = perturbation_patches_.find(GetCurrentBlockIndex());
    if (patches_iter == perturbation_patches_.end())
    {
        return data;
    }

    perturbation_buffer_.assign(data, data + size);
    for (const auto& patch : patches_iter->second)
    {
        if (patch.first < base_offset)
        {
            continue;
        }
        uint64_t relative_offset = patch.first - base_offset;
        if ((relative_offset + sizeof(patch.second)) <= size)
        {
            util::platform::MemoryCopy(perturbation_buffer_.data() + relative_offset,
                                       sizeof(patch.second),
                                       &patch.second,
                                       sizeof(patch.second));
        }
    }
    return perturbation_buffer_.data();
}

void Dx12ResourceValueTrackingConsumer::ProcessFillMemoryCommand(uint64_t       memory_id,
                                                                 uint64_t       offset,
                                                                 uint64_t       size,
                                                                 const uint8_t* data)
{
    data = ApplyResourceValuePerturbation(offset, size, data);
    Dx12ReplayConsumer::ProcessFillMemoryCommand(memory_id, offset, size, data);
}

void Dx12ResourceValueTrackingConsumer::ProcessInitSubresourceCommand(
    const format::InitSubresourceCommandHeader& command_header, const uint8_t* data)
{
    data = ApplyResourceValuePerturbation(0, command_header.data_size, data);
    Dx12ReplayConsumer::ProcessInitSubresourceCommand(command_header, data);
}

void Dx12ResourceValueTrackingConsumer::Process_ID3D12GraphicsCommandList4_CopyRaytracingAccelerationStructure(
    const ApiCallInfo&                                call_info,
    format::HandleId                                  object_id,
    D3D12_GPU_VIRTUAL_ADDRESS                         DestAccelerationStructureData,
    D3D12_GPU_VIRTUAL_ADDRESS                         SourceAccelerationStructureData,
    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE Mode)
{
    if (replay_resource_value_calls_)
    {
        Dx12ReplayConsumer::Process_ID3D12GraphicsCommandList4_CopyRaytracingAccelerationStructure(
            call_info, object_id, DestAccelerationStructureData, SourceAccelerationStructureData, Mode);
    }
}

void Dx12ResourceValueTrackingConsumer::ProcessInitDx12AccelerationStructureCommand(
    const format::InitDx12AccelerationStructureCommandHeader&                           command_header,
    const std::vector<format::InitDx12AccelerationStructureGeometryDesc>&               geometry_descs,
    StructPointerDecoder<Decoded_D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS>* build_inputs,
    const uint8_t*                                                                      build_inputs_data)
{
    dxr_workload_ = true;

    // Execute state-load AS init even when DXR replay is disabled: its build inputs are handle mapped (no
    // runtime mapping dependency), and valid state-load structures keep unskippable inline raytracing off
    // uninitialized memory. Frame builds remain skipped; their stale top levels can only reference
    // state-load bottom levels, so never-built structures stay unreachable.
    Dx12ReplayConsumer::ProcessInitDx12AccelerationStructureCommand(
        command_header, geometry_descs, build_inputs, build_inputs_data);
}

void Dx12ResourceValueTrackingConsumer::Process_ID3D12GraphicsCommandList4_EmitRaytracingAccelerationStructurePostbuildInfo(
    const ApiCallInfo&                                                                          call_info,
    format::HandleId                                                                            object_id,
    StructPointerDecoder<Decoded_D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC>*  pDesc,
    UINT                                                                                        NumSourceAccelerationStructures,
    PointerDecoder<D3D12_GPU_VIRTUAL_ADDRESS>*                                                  pSourceAccelerationStructureData)
{
    // Postbuild info reads the source structures' internal headers on the GPU, which faults on structures
    // whose builds were skipped; its output only feeds compaction copies that are skipped with it.
    if (replay_resource_value_calls_)
    {
        Dx12ReplayConsumer::Process_ID3D12GraphicsCommandList4_EmitRaytracingAccelerationStructurePostbuildInfo(
            call_info, object_id, pDesc, NumSourceAccelerationStructures, pSourceAccelerationStructureData);
    }
}

void Dx12ResourceValueTrackingConsumer::OverrideExecuteIndirect(DxObjectInfo* command_list_object_info,
                                                                DxObjectInfo* command_signature_object_info,
                                                                UINT          max_command_count,
                                                                DxObjectInfo* argument_buffer_object_info,
                                                                UINT64        argument_buffer_offset,
                                                                DxObjectInfo* count_buffer_object_info,
                                                                UINT64        count_buffer_offset)
{
    ei_workload_ = true;

    auto command_list      = static_cast<ID3D12GraphicsCommandList*>(command_list_object_info->object);
    auto command_signature = static_cast<ID3D12CommandSignature*>(command_signature_object_info->object);
    auto argument_buffer   = static_cast<ID3D12Resource*>(argument_buffer_object_info->object);

    ID3D12Resource* count_buffer = nullptr;
    if (count_buffer_object_info != nullptr)
    {
        count_buffer = static_cast<ID3D12Resource*>(count_buffer_object_info->object);
    }

    auto command_list_extra_info      = GetExtraInfo<D3D12CommandListInfo>(command_list_object_info);
    auto command_signature_extra_info = GetExtraInfo<D3D12CommandSignatureInfo>(command_signature_object_info);

    // Classify the call by its own signature. The command list's resource_value_info_map is not usable here:
    // PostProcessExecuteIndirect files this call's entry after this override runs, and count-buffer calls
    // file it under the count buffer, so checking the argument-buffer entry misclassifies both.
    bool consumes_resource_values = !command_signature_extra_info->resource_value_infos.empty();

    if (!consumes_resource_values)
    {
        // This call executes in every tracking pass. Record the argument/count ranges so the perturbation
        // pass never tags candidate bytes an executed indirect call would consume.
        uint64_t argument_span = static_cast<uint64_t>(max_command_count) * command_signature_extra_info->byte_stride;
        non_rv_ei_ranges_[argument_buffer_object_info->capture_id].insert(
            { argument_buffer_offset, argument_buffer_offset + argument_span });
        if (count_buffer_object_info != nullptr)
        {
            non_rv_ei_ranges_[count_buffer_object_info->capture_id].insert(
                { count_buffer_offset, count_buffer_offset + sizeof(uint32_t) });
        }
    }

    if (!consumes_resource_values || replay_resource_value_calls_)
    {
        command_list->ExecuteIndirect(command_signature,
                                      max_command_count,
                                      argument_buffer,
                                      argument_buffer_offset,
                                      count_buffer,
                                      count_buffer_offset);
    }

    command_list_extra_info->requires_sync_after_execute = true;

    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->PostProcessExecuteIndirect(command_list_object_info,
                                                             command_signature_object_info,
                                                             max_command_count,
                                                             argument_buffer_object_info,
                                                             argument_buffer_offset,
                                                             count_buffer_object_info,
                                                             count_buffer_offset);
    }
}

void Dx12ResourceValueTrackingConsumer::OverrideBuildRaytracingAccelerationStructure(
    DxObjectInfo*                                                                     command_list4_object_info,
    StructPointerDecoder<Decoded_D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC>* desc,
    UINT                                                                              num_post_build_info_descs,
    StructPointerDecoder<Decoded_D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC>* post_build_info_descs)
{
    GFXRECON_ASSERT(command_list4_object_info != nullptr);
    GFXRECON_ASSERT(command_list4_object_info->object != nullptr);

    dxr_workload_ = true;

    auto command_list4 = static_cast<ID3D12GraphicsCommandList4*>(command_list4_object_info->object);

    if (replay_resource_value_calls_)
    {
        command_list4->BuildRaytracingAccelerationStructure(
            desc->GetPointer(), num_post_build_info_descs, post_build_info_descs->GetPointer());
    }

    auto command_list_extra_info = GetExtraInfo<D3D12CommandListInfo>(command_list4_object_info);
    command_list_extra_info->requires_sync_after_execute = true;

    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->PostProcessBuildRaytracingAccelerationStructure(command_list4_object_info, desc);
    }
}

void Dx12ResourceValueTrackingConsumer::OverrideDispatchRays(
    DxObjectInfo* command_list4_object_info, StructPointerDecoder<Decoded_D3D12_DISPATCH_RAYS_DESC>* desc_decoder)
{
    auto command_list4 = static_cast<ID3D12GraphicsCommandList4*>(command_list4_object_info->object);

    if (replay_resource_value_calls_)
    {
        command_list4->DispatchRays(desc_decoder->GetPointer());
    }

    auto command_list_extra_info = GetExtraInfo<D3D12CommandListInfo>(command_list4_object_info);
    command_list_extra_info->requires_sync_after_execute = true;

    if (GetResourceValueMapper() != nullptr)
    {
        GetResourceValueMapper()->PostProcessDispatchRays(command_list4_object_info, desc_decoder);
    }
}

GFXRECON_END_NAMESPACE(decode)
GFXRECON_END_NAMESPACE(gfxrecon)
