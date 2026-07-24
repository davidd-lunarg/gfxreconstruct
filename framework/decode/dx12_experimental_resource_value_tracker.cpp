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

#include "decode/dx12_experimental_resource_value_tracker.h"

#include "decode/custom_dx12_struct_decoders.h"

#include <algorithm>
#include <map>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(decode)

bool Dx12ExperimentalResourceValueTracker::AddTrackedResourceValue(format::HandleId              resource_id,
                                                                   ResourceValueType             type,
                                                                   uint64_t                      offset,
                                                                   const uint8_t*                resource_value_data,
                                                                   const graphics::Dx12GpuVaMap& gpu_va_map)
{
    bool success =
        Dx12ResourceValueTracker::AddTrackedResourceValue(resource_id, type, offset, resource_value_data, gpu_va_map);

    if (!success && track_unassociated_values_)
    {
        uint64_t block_index                  = get_current_block_index_func_();
        auto&    unassociated_resource_values = unassociated_resource_values_[resource_id];
        GFXRECON_ASSERT(unassociated_resource_values.empty() ||
                        (block_index >= unassociated_resource_values.back().block_index));

        if (unassociated_resource_values.empty() || (block_index > unassociated_resource_values.back().block_index))
        {
            unassociated_resource_values.push_back({ block_index, {} });
        }
        auto& values_group = unassociated_resource_values.back();

        Dx12UnassociatedResourceValue unassociated_value;
        unassociated_value.type = type;

        if (type == ResourceValueType::kShaderIdentifier)
        {
            unassociated_value.shader_id = graphics::PackDx12ShaderIdentifier(resource_value_data);
        }
        else if (type == ResourceValueType::kGpuVirtualAddress)
        {
            auto     referenced_resource_id = format::kNullHandleId;
            bool     found_resource_id      = false;
            uint64_t old_address;
            util::platform::MemoryCopy(&old_address, 8, resource_value_data, 8);
            gpu_va_map.Map(old_address, &referenced_resource_id, &found_resource_id);
            if (found_resource_id)
            {
                unassociated_value.resource_id = referenced_resource_id;
            }
            else
            {
                GFXRECON_LOG_ERROR("Failed to find the resource for an unassociated resource GPU VA (address=%" PRIu64
                                   ").",
                                   old_address);
            }
        }

        values_group.values.push_back(std::move(unassociated_value));
    }

    if (resolve_unassociated_values_)
    {
        AuditResourceValue(resource_id, type, resource_value_data, gpu_va_map, success);
    }

    return success;
}

void Dx12ExperimentalResourceValueTracker::AuditResourceValue(format::HandleId              resource_id,
                                                              ResourceValueType             type,
                                                              const uint8_t*                resource_value_data,
                                                              const graphics::Dx12GpuVaMap& gpu_va_map,
                                                              bool                          walk_attributed)
{
    Dx12ResourceValueAuditCounts* counts = nullptr;
    switch (type)
    {
        case ResourceValueType::kGpuVirtualAddress:
            counts = &audit_summary_.gpu_va;
            break;
        case ResourceValueType::kGpuDescriptorHandle:
            counts = &audit_summary_.descriptor_handle;
            break;
        case ResourceValueType::kShaderIdentifier:
            counts = &audit_summary_.shader_id;
            break;
        default:
            return;
    }

    ++counts->total;

    if (walk_attributed)
    {
        ++counts->satisfied_by_walk;
        return;
    }

    if (type == ResourceValueType::kShaderIdentifier)
    {
        auto shader_id = graphics::PackDx12ShaderIdentifier(resource_value_data);
        if (scanned_candidate_shader_ids_.count(shader_id) > 0)
        {
            ++counts->covered_by_candidate;
        }
        else
        {
            ++counts->unresolved;
            unresolved_shader_id_values_.insert(shader_id);
            ++audit_summary_.unresolved_by_resource[resource_id];
        }
        return;
    }

    uint64_t value = 0;
    util::platform::MemoryCopy(&value, sizeof(value), resource_value_data, sizeof(value));

    format::HandleId target_resource_id = format::kNullHandleId;
    if (type == ResourceValueType::kGpuVirtualAddress)
    {
        bool found = false;
        gpu_va_map.Map(value, &target_resource_id, &found);
        if (!found)
        {
            // The capture address falls in no tracked allocation: an application-side stale pointer into
            // memory freed before the capture range. The runtime mapper also cannot map these, so no
            // annotation can ever cover them and the correct optimized emission is the unchanged value.
            ++counts->dead_capture_va;
            dead_capture_va_values_.insert(value);
            return;
        }
    }

    const auto& candidates =
        (type == ResourceValueType::kGpuVirtualAddress) ? scanned_candidate_gpu_vas_ : scanned_candidate_descriptors_;
    if (candidates.count(value) > 0)
    {
        ++counts->covered_by_candidate;
    }
    else
    {
        ++counts->unresolved;
        if (type == ResourceValueType::kGpuVirtualAddress)
        {
            ++audit_summary_.unresolved_by_target_resource[target_resource_id];
            if (unresolved_gpu_va_values_.insert(value).second &&
                (audit_summary_.unresolved_gpu_va_samples.size() < kMaxUnresolvedValueSamples))
            {
                audit_summary_.unresolved_gpu_va_samples.push_back(
                    { value, target_resource_id, get_current_block_index_func_() });
            }
        }
        ++audit_summary_.unresolved_by_resource[resource_id];
    }
}

void Dx12ExperimentalResourceValueTracker::GetAuditSummary(Dx12ResourceValueAuditSummary& summary)
{
    summary                                = audit_summary_;
    summary.distinct_unresolved_gpu_vas    = unresolved_gpu_va_values_.size();
    summary.distinct_dead_capture_vas      = dead_capture_va_values_.size();
    summary.distinct_unresolved_shader_ids = unresolved_shader_id_values_.size();
}

void Dx12ExperimentalResourceValueTracker::GetTrackedResourceValues(Dx12FillCommandResourceValueMap& tracked_values)
{
    Dx12ResourceValueTracker::GetTrackedResourceValues(tracked_values);

    if (resolve_unassociated_values_)
    {
        for (auto& non_dxr_ranges_pair : non_dxr_fill_command_data_)
        {
            auto block_index                = non_dxr_ranges_pair.first;
            auto block_resource_values_iter = tracked_values.find(block_index);
            if (block_resource_values_iter != tracked_values.end())
            {
                auto& res_vals = block_resource_values_iter->second;
                for (auto& non_dxr_range : non_dxr_ranges_pair.second)
                {
                    auto min_offset = non_dxr_range.first;
                    auto max_offset = non_dxr_range.second;
                    auto end_iter =
                        std::remove_if(res_vals.begin(),
                                       res_vals.end(),
                                       [min_offset, max_offset](const Dx12FillCommandResourceValue& res_val) {
                                           return (res_val.offset >= min_offset) && (res_val.offset < max_offset);
                                       });
                    res_vals.erase(end_iter, res_vals.end());
                }

                // Erase only after all ranges are applied; erasing inside the range loop invalidates
                // block_resource_values_iter for the next range.
                if (res_vals.empty())
                {
                    tracked_values.erase(block_resource_values_iter);
                }
            }
        }
    }
}

void Dx12ExperimentalResourceValueTracker::PostProcessExecuteCommandLists(
    DxObjectInfo*                             command_queue_object_info,
    UINT                                      num_command_lists,
    HandlePointerDecoder<ID3D12CommandList*>* command_lists_decoder)
{
    auto command_queue_extra_info = GetExtraInfo<D3D12CommandQueueInfo>(command_queue_object_info);

    GFXRECON_ASSERT(command_queue_extra_info != nullptr);

    auto& pending_events = command_queue_extra_info->pending_events;

    ProcessExecuteCommandListArgs process_args{};

    for (UINT i = 0; i < num_command_lists; ++i)
    {
        auto command_list_id         = command_lists_decoder->GetPointer()[i];
        auto command_list_extra_info = GetExtraInfo<D3D12CommandListInfo>(get_object_info_func_(command_list_id));
        GFXRECON_ASSERT(command_list_extra_info != nullptr);

        process_args.resource_copies.insert(process_args.resource_copies.end(),
                                            command_list_extra_info->resource_copies.begin(),
                                            command_list_extra_info->resource_copies.end());

        auto tracker_command_list_info = tracker_command_list_infos_[command_list_id];

        for (const auto& non_dxr_resource_pair : tracker_command_list_info.non_dxr_resources_)
        {
            auto& args_vec = process_args.non_dxr_resources[non_dxr_resource_pair.first];
            args_vec.insert(args_vec.end(), non_dxr_resource_pair.second.begin(), non_dxr_resource_pair.second.end());
        }
    }

    // Run (or queue) resource value mapping process.
    if (pending_events.empty())
    {
        ProcessExecuteCommandList(std::move(process_args));
    }
    else
    {
        command_queue_extra_info->pending_events.push_back(
            CreateProcessExecuteCommandListSyncEvent(std::move(process_args)));
    }
}

void Dx12ExperimentalResourceValueTracker::PostProcessFillMemoryCommand(uint64_t       resource_id,
                                                                        uint64_t       offset,
                                                                        uint64_t       size,
                                                                        const uint8_t* data)
{
    TrackedFillCommandInfo tracked_fill_command;
    tracked_fill_command.fill_command_block_index = get_current_block_index_func_();
    tracked_fill_command.original_offset          = offset;
    tracked_fill_command.offset                   = offset;
    tracked_fill_command.size                     = size;

    Dx12ResourceValueTracker::PostProcessFillMemoryCommand(resource_id, offset, size, data);

    if (resolve_unassociated_values_)
    {
        auto resource_object_info = get_object_info_func_(resource_id);
        auto resource             = static_cast<ID3D12Resource*>(resource_object_info->object);
        if (resource->GetDesc().Dimension != D3D12_RESOURCE_DIMENSION_BUFFER)
        {
            return;
        }

        GFXRECON_ASSERT((offset % kMinDataAlignment) == 0);
        FindResourceValuesThreaded(tracked_fill_command, data, size);
    }
}

void Dx12ExperimentalResourceValueTracker::PostProcessInitSubresourceCommand(
    ID3D12Resource* resource, const format::InitSubresourceCommandHeader& command_header, const uint8_t* data)
{
    GFXRECON_ASSERT(resource != nullptr);

    // Only support buffer resources.
    if (resource->GetDesc().Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
    {
        // Buffer resources only have subresource 0.
        GFXRECON_ASSERT(command_header.subresource == 0);

        TrackedFillCommandInfo tracked_fill_command;
        tracked_fill_command.fill_command_block_index = get_current_block_index_func_();
        tracked_fill_command.original_offset          = 0;
        tracked_fill_command.offset                   = 0;
        tracked_fill_command.size                     = command_header.data_size;
        tracked_fill_command.init_subresource_data =
            std::make_shared<std::vector<uint8_t>>(std::vector<uint8_t>(data, data + command_header.data_size));

        if (resolve_unassociated_values_)
        {
            FindResourceValuesThreaded(tracked_fill_command, data, command_header.data_size);
        }

        UpdateFillCommandState(command_header.resource_id, tracked_fill_command);
    }
}

void Dx12ExperimentalResourceValueTracker::ProcessExecuteCommandList(ProcessExecuteCommandListArgs args)
{
    // Process non-DXR resource data ranges.
    for (const auto& non_dxr_resources_pair : args.non_dxr_resources)
    {
        auto resource_id = non_dxr_resources_pair.first;
        for (const auto& non_dxr_range : non_dxr_resources_pair.second)
        {
            AddNonDxrFillCommandBlocks(resource_id, non_dxr_range.first, non_dxr_range.second);
        }
    }

    Dx12ResourceValueTracker::ProcessExecuteCommandList(std::move(args));
}

bool MatchShaderIdentifier(const std::set<graphics::Dx12ShaderIdentifier>& shader_id_set, const uint8_t* data)
{
    for (const auto& shader_id : shader_id_set)
    {
        bool match = true;
        for (uint64_t i = 0; (i < D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) && match; ++i)
        {
            match = match && (shader_id.data()[i] == data[i]);
        }
        if (match)
        {
            return true;
        }
    }
    return false;
}

void Dx12ExperimentalResourceValueTracker::FindResourceValues(
    const uint8_t*                                               data,
    uint64_t                                                     data_size,
    uint64_t                                                     record_limit,
    const std::set<graphics::Dx12ShaderIdentifier>*              shader_ids,
    const graphics::Dx12GpuVaMap*                                gpu_va_map,
    const decode::Dx12DescriptorMap*                             gpu_descriptor_map,
    std::vector<std::pair<uint64_t, format::ResourceValueType>>* found_resource_values) const
{
    found_resource_values->clear();

    const uint64_t kDescSize = sizeof(D3D12_GPU_DESCRIPTOR_HANDLE::ptr);
    const uint64_t kAddrSize = sizeof(uint64_t);
    const uint64_t kIdSize   = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;

    if ((record_limit == 0) || (record_limit > data_size))
    {
        record_limit = data_size;
    }

    for (uint64_t i = 0; (i + kMinDataAlignment) <= record_limit; i += kMinDataAlignment)
    {
        // First check for a shader id match.
        if ((shader_ids != nullptr) && ((i + kIdSize) <= data_size))
        {
            if (MatchShaderIdentifier(*shader_ids, data + i))
            {
                found_resource_values->emplace_back(i, ResourceValueType::kShaderIdentifier);
                i += kIdSize - kMinDataAlignment;
                continue;
            }
        }

        // Next check for GPU descriptor match.
        if ((gpu_descriptor_map != nullptr) && ((i + kDescSize) <= data_size))
        {
            D3D12_GPU_DESCRIPTOR_HANDLE old_descriptor;
            util::platform::MemoryCopy(&old_descriptor.ptr, kDescSize, data + i, kDescSize);
            if ((old_descriptor.ptr >= min_gpu_descriptor_) && (old_descriptor.ptr < max_gpu_descriptor_) &&
                (old_descriptor.ptr % min_gpu_descriptor_alignment_ == 0))
            {
                bool found_descriptor = false;
                gpu_descriptor_map->GetGpuAddress(old_descriptor, &found_descriptor);
                if (found_descriptor)
                {
                    found_resource_values->emplace_back(i, ResourceValueType::kGpuDescriptorHandle);
                    i += kDescSize - kMinDataAlignment;
                    continue;
                }
            }
        }

        // No value-alignment gate on VAs: record arguments can point at arbitrary structure elements (19% of
        // real record VAs measured on a large DXR title are not 8-byte aligned). The range gate and the VA map
        // lookup are the filters.
        if ((gpu_va_map != nullptr) && ((i + kAddrSize) <= data_size))
        {

            uint64_t old_address;
            util::platform::MemoryCopy(&old_address, kAddrSize, data + i, kAddrSize);
            if ((old_address >= min_gpu_va_) && (old_address < max_gpu_va_))
            {
                bool found_address = false;
                gpu_va_map->Map(old_address, nullptr, &found_address);
                if (found_address)
                {
                    found_resource_values->emplace_back(i, ResourceValueType::kGpuVirtualAddress);
                    i += kAddrSize - kMinDataAlignment;
                    continue;
                }
            }
        }
    }
}

void Dx12ExperimentalResourceValueTracker::FindResourceValuesThreaded(
    const TrackedFillCommandInfo& tracked_fill_command, const uint8_t* data, uint64_t data_size)
{
    const uint64_t kThreadCount = 8;

    if (find_resource_values_thread_datas_.size() < kThreadCount)
    {
        find_resource_values_thread_datas_.resize(kThreadCount);
    }

    const uint64_t kMinPerThreadSize = 512;
    uint64_t       per_thread_size   = util::platform::AlignValue<D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES>(
        std::max(data_size / kThreadCount, kMinPerThreadSize));

    // Threads read past their chunk so boundary-straddling values are found by the chunk owning their start;
    // values may only start inside the chunk (record_limit), so the overlap produces no duplicate hits.
    const uint64_t kChunkOverlap = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES - kMinDataAlignment;

    uint64_t next_thread_offset = 0;
    for (auto& thread_data : find_resource_values_thread_datas_)
    {
        thread_data.data_offset = next_thread_offset;
        thread_data.data_size   = 0;
        if (data_size > next_thread_offset)
        {
            thread_data.data_size = std::min(per_thread_size, data_size - next_thread_offset);
        }
        next_thread_offset += thread_data.data_size;

        if (thread_data.data_size > 0)
        {
            uint64_t scan_size = std::min(thread_data.data_size + kChunkOverlap, data_size - thread_data.data_offset);
            thread_data.thread = std::thread(&Dx12ExperimentalResourceValueTracker::FindResourceValues,
                                             this,
                                             data + thread_data.data_offset,
                                             scan_size,
                                             thread_data.data_size,
                                             &unique_unassociated_shader_ids_,
                                             &active_unassociated_gpu_va_map_,
                                             &active_unassociated_gpu_descriptor_map_,
                                             &thread_data.found_resource_values);
        }
    }

    for (auto& thread_data : find_resource_values_thread_datas_)
    {
        if (thread_data.data_size > 0)
        {
            thread_data.thread.join();
            for (const auto& found_resource_value : thread_data.found_resource_values)
            {
                // Record the found value's content so the audit ledger can classify use-site observations
                // as covered by a content-scan candidate.
                const uint8_t* value_data = data + thread_data.data_offset + found_resource_value.first;
                if (found_resource_value.second == ResourceValueType::kShaderIdentifier)
                {
                    scanned_candidate_shader_ids_.insert(graphics::PackDx12ShaderIdentifier(value_data));
                }
                else
                {
                    uint64_t value = 0;
                    util::platform::MemoryCopy(&value, sizeof(value), value_data, sizeof(value));
                    if (found_resource_value.second == ResourceValueType::kGpuVirtualAddress)
                    {
                        scanned_candidate_gpu_vas_.insert(value);
                    }
                    else
                    {
                        scanned_candidate_descriptors_.insert(value);
                    }
                }

                AddBlockResourceValue(tracked_fill_command.fill_command_block_index,
                                      tracked_fill_command.original_offset + thread_data.data_offset +
                                          found_resource_value.first,
                                      found_resource_value.second);
            }
        }
    }
}

void Dx12ExperimentalResourceValueTracker::GetUnassociatedResourceValues(
    Dx12UnassociatedResourceValueMap& unassociated_values)
{
    if (track_unassociated_values_)
    {
        unassociated_values = std::move(unassociated_resource_values_);
        unassociated_resource_values_.clear();
    }
}

void Dx12ExperimentalResourceValueTracker::SetUnassociatedResourceValues(
    Dx12FillCommandResourceValueMap&& tracked_values, decode::Dx12UnassociatedResourceValueMap&& unassociated_values)
{
    GFXRECON_ASSERT(unassociated_resource_values_.empty());

    track_unassociated_values_   = false;
    resolve_unassociated_values_ = true;

    tracked_resource_values_      = std::move(tracked_values);
    unassociated_resource_values_ = std::move(unassociated_values);

    for (const auto& unassociated_resource_values_map : unassociated_resource_values_)
    {
        for (const auto& unassociated_resource_value_groups : unassociated_resource_values_map.second)
        {
            for (const auto& unassociated_resource_value : unassociated_resource_value_groups.values)
            {
                if (unassociated_resource_value.type == ResourceValueType::kShaderIdentifier)
                {
                    unique_unassociated_shader_ids_.insert(unassociated_resource_value.shader_id);
                }
                else if (unassociated_resource_value.type == ResourceValueType::kGpuVirtualAddress)
                {
                    if (unassociated_resource_value.resource_id != format::kNullHandleId)
                    {
                        all_unassociated_gpu_va_map_.insert(unassociated_resource_value.resource_id);
                    }
                }
                else if (unassociated_resource_value.type == ResourceValueType::kGpuDescriptorHandle)
                {
                    // TODO: Add heap info to Dx12UnassociatedResourceValue and fill in
                    // all_unassociated_gpu_descriptor_map_ here, then use that to limit GPU descriptor search.
                }
            }
        }
    }
}

void Dx12ExperimentalResourceValueTracker::AddShaderRecordData(format::HandleId                 resource_id,
                                                               uint64_t                         offset,
                                                               uint64_t                         shader_record_size,
                                                               const uint8_t*                   shader_record_data,
                                                               const graphics::Dx12GpuVaMap&    gpu_va_map,
                                                               const decode::Dx12DescriptorMap& descriptor_map)
{
    if (track_unassociated_values_ || resolve_unassociated_values_)
    {
        temp_found_shader_record_values_.clear();
        FindResourceValues(shader_record_data,
                           shader_record_size,
                           shader_record_size,
                           nullptr,
                           &gpu_va_map,
                           &descriptor_map,
                           &temp_found_shader_record_values_);
        for (const auto& found_resource_value : temp_found_shader_record_values_)
        {
            AddTrackedResourceValue(resource_id,
                                    found_resource_value.second,
                                    found_resource_value.first + offset,
                                    shader_record_data + found_resource_value.first,
                                    gpu_va_map);
        }
    }
}

void Dx12ExperimentalResourceValueTracker::AddNonDxrFillCommandBlocks(format::HandleId resource_id,
                                                                      uint64_t         resource_min_offset,
                                                                      uint64_t         resource_max_offset)
{
    auto& resource_fill_commands = tracked_fill_commands_[resource_id];

    auto iter = resource_fill_commands.begin();
    while (iter != resource_fill_commands.end())
    {
        auto next_iter = iter;
        ++next_iter;

        // If the incoming non DXR data region overlaps with tracked fill commands, compute the offsets in the original
        // fill command that the non DXR data came from and save to non_dxr_fill_command_data_.
        uint64_t fill_min_offset = iter->second.offset;
        uint64_t fill_max_offset = fill_min_offset + iter->second.size;
        bool     overlap         = (fill_min_offset < resource_max_offset) && (fill_max_offset > resource_min_offset);
        if (overlap)
        {
            uint64_t offset_min = std::max(resource_min_offset, fill_min_offset);
            uint64_t offset_max = std::min(resource_max_offset, fill_max_offset);

            uint64_t original_min = iter->second.original_offset + offset_min - iter->second.offset;
            uint64_t original_max = iter->second.original_offset + offset_max - iter->second.offset;

            auto& non_dxr_ranges         = non_dxr_fill_command_data_[iter->second.fill_command_block_index];
            non_dxr_ranges[original_min] = original_max;
        }

        iter = next_iter;
    }
}

void Dx12ExperimentalResourceValueTracker::PostProcessCopyTextureRegion(
    DxObjectInfo*                                              command_list_object_info,
    StructPointerDecoder<Decoded_D3D12_TEXTURE_COPY_LOCATION>* dst_decoder,
    UINT                                                       dst_x,
    UINT                                                       dst_y,
    UINT                                                       dst_z,
    StructPointerDecoder<Decoded_D3D12_TEXTURE_COPY_LOCATION>* src_decoder,
    StructPointerDecoder<Decoded_D3D12_BOX>*                   src_box_decoder)
{
    if (resolve_unassociated_values_)
    {
        auto dst_copy_location = dst_decoder->GetPointer();
        auto src_copy_location = src_decoder->GetPointer();
        if ((dst_copy_location->Type == D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX) &&
            (src_copy_location->Type == D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT))
        {
            auto resource = dst_copy_location->pResource;
            if (resource != nullptr)
            {
                auto        device           = graphics::dx12::GetDeviceComPtrFromChild<ID3D12Device>(resource);
                const auto& placed_footprint = src_copy_location->PlacedFootprint;
                uint64_t    copy_size        = 0;
                auto        desc             = resource->GetDesc();
                device->GetCopyableFootprints(&desc,
                                              dst_copy_location->SubresourceIndex,
                                              1,
                                              placed_footprint.Offset,
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              &copy_size);

                auto  resource_id               = src_decoder->GetMetaStructPointer()->pResource;
                auto& tracker_command_list_info = tracker_command_list_infos_[command_list_object_info->capture_id];
                tracker_command_list_info.non_dxr_resources_[resource_id].push_back(
                    { placed_footprint.Offset, placed_footprint.Offset + copy_size });
            }
        }
    }
}

void Dx12ExperimentalResourceValueTracker::PostProcessIASetIndexBuffer(
    DxObjectInfo*                                          command_list_object_info,
    StructPointerDecoder<Decoded_D3D12_INDEX_BUFFER_VIEW>* views_decoder,
    const graphics::Dx12GpuVaMap&                          reverse_gpu_va_map)
{
    if (resolve_unassociated_values_)
    {
        auto index_buffer_view = views_decoder->GetPointer();
        if (index_buffer_view != nullptr)
        {
            auto resource_id = format::kNullHandleId;
            reverse_gpu_va_map.Map(index_buffer_view->BufferLocation, &resource_id);
            if (resource_id != format::kNullHandleId)
            {
                auto resource_object_info = get_object_info_func_(resource_id);
                auto resource             = static_cast<ID3D12Resource*>(resource_object_info->object);
                auto buffer_offset        = index_buffer_view->BufferLocation - resource->GetGPUVirtualAddress();

                auto& tracker_command_list_info = tracker_command_list_infos_[command_list_object_info->capture_id];
                tracker_command_list_info.non_dxr_resources_[resource_id].push_back(
                    { buffer_offset, buffer_offset + index_buffer_view->SizeInBytes });
            }
        }
    }
}

void Dx12ExperimentalResourceValueTracker::PostProcessIASetVertexBuffers(
    DxObjectInfo*                                           command_list_object_info,
    UINT                                                    start_slot,
    UINT                                                    num_views,
    StructPointerDecoder<Decoded_D3D12_VERTEX_BUFFER_VIEW>* views_decoder,
    const graphics::Dx12GpuVaMap&                           reverse_gpu_va_map)
{
    if (resolve_unassociated_values_)
    {
        auto vertex_buffer_view = views_decoder->GetPointer();
        if (vertex_buffer_view != nullptr)
        {
            auto resource_id = format::kNullHandleId;
            reverse_gpu_va_map.Map(vertex_buffer_view->BufferLocation, &resource_id);
            if (resource_id != format::kNullHandleId)
            {
                auto resource_object_info = get_object_info_func_(resource_id);
                auto resource             = static_cast<ID3D12Resource*>(resource_object_info->object);
                auto buffer_offset        = vertex_buffer_view->BufferLocation - resource->GetGPUVirtualAddress();

                auto& tracker_command_list_info = tracker_command_list_infos_[command_list_object_info->capture_id];
                tracker_command_list_info.non_dxr_resources_[resource_id].push_back(
                    { buffer_offset, buffer_offset + vertex_buffer_view->SizeInBytes });
            }
        }
    }
}

void Dx12ExperimentalResourceValueTracker::PostProcessCommandListReset(DxObjectInfo* command_list_object_info)
{
    tracker_command_list_infos_[command_list_object_info->capture_id].Reset();
}

void Dx12ExperimentalResourceValueTracker::AddResourceGpuVa(format::HandleId          resource_id,
                                                            D3D12_GPU_VIRTUAL_ADDRESS replay_address,
                                                            UINT64                    width,
                                                            D3D12_GPU_VIRTUAL_ADDRESS capture_address)
{
    if (track_unassociated_values_)
    {
        min_gpu_va_ = std::min(min_gpu_va_, capture_address);
        max_gpu_va_ = std::max(max_gpu_va_, capture_address + width);
    }
    else if (all_unassociated_gpu_va_map_.count(resource_id) > 0)
    {
        active_unassociated_gpu_va_map_.Add(resource_id, capture_address, width, replay_address);
        min_gpu_va_ = std::min(min_gpu_va_, capture_address);
        max_gpu_va_ = std::max(max_gpu_va_, capture_address + width);
    }
}

void Dx12ExperimentalResourceValueTracker::RemoveResourceGpuVa(format::HandleId resource_id, uint64_t capture_address)
{
    active_unassociated_gpu_va_map_.Remove(resource_id, capture_address);
}

void Dx12ExperimentalResourceValueTracker::AddGpuDescriptorHeap(
    const D3D12_GPU_DESCRIPTOR_HANDLE&     capture_gpu_start,
    const D3D12_GPU_DESCRIPTOR_HANDLE&     replay_gpu_start,
    D3D12_DESCRIPTOR_HEAP_TYPE             heap_info_descriptor_type,
    uint32_t                               heap_info_descriptor_count,
    std::shared_ptr<DescriptorIncrements>& heap_info_capture_increments,
    std::shared_ptr<DescriptorIncrements>& heap_info_replay_increments)
{
    min_gpu_descriptor_ = std::min(min_gpu_descriptor_, capture_gpu_start.ptr);
    max_gpu_descriptor_ =
        std::max(max_gpu_descriptor_,
                 capture_gpu_start.ptr + static_cast<uint64_t>(heap_info_descriptor_count) *
                                             (*heap_info_capture_increments)[heap_info_descriptor_type]);

    if (resolve_unassociated_values_)
    {
        active_unassociated_gpu_descriptor_map_.AddGpuDescriptorHeap(capture_gpu_start,
                                                                     replay_gpu_start,
                                                                     heap_info_descriptor_type,
                                                                     heap_info_descriptor_count,
                                                                     heap_info_capture_increments,
                                                                     heap_info_replay_increments);
    }

    for (auto increment : *heap_info_capture_increments)
    {
        if (increment > 0)
        {
            min_gpu_descriptor_alignment_ = std::min(min_gpu_descriptor_alignment_, static_cast<uint64_t>(increment));
        }
    }
}

void Dx12ExperimentalResourceValueTracker::RemoveGpuDescriptorHeap(uint64_t capture_gpu_start)
{
    active_unassociated_gpu_descriptor_map_.RemoveGpuDescriptorHeap(capture_gpu_start);
}

GFXRECON_END_NAMESPACE(decode)
GFXRECON_END_NAMESPACE(gfxrecon)
