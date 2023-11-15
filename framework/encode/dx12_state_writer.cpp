/*
** Copyright (c) 2021-2023 LunarG, Inc.
** Copyright (c) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
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

#include "encode/dx12_state_writer.h"

#include "generated/generated_dx12_api_call_encoders.h"
#include "encode/custom_dx12_struct_encoders.h"
#include "encode/custom_dx12_struct_unwrappers.h"
#include "encode/struct_pointer_encoder.h"
#include "encode/d3d12_capture_manager.h"
#include "graphics/dx12_resource_data_util.h"
#include "graphics/dx12_util.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

Dx12StateWriter::Dx12StateWriter(util::FileOutputStream* output_stream,
                                 util::Compressor*       compressor,
                                 format::ThreadId        thread_id) :
    Dx12StateWriterBase(output_stream, compressor, thread_id),
    saved_state_(nullptr)
{
    assert(output_stream != nullptr);
}

Dx12StateWriter::~Dx12StateWriter() {}

#ifdef GFXRECON_AGS_SUPPORT
void Dx12StateWriter::WriteState(const Dx12StateTable& state_table,
                                 const AgsStateTable&  ags_state_table,
                                 uint64_t              frame_number,
                                 Dx12SavedState*       saved_state)
#else
void Dx12StateWriter::WriteState(const Dx12StateTable& state_table, uint64_t frame_number, Dx12SavedState* saved_state)
#endif // GFXRECON_AGS_SUPPORT
{
    saved_state_ = saved_state;

#if GFXRECON_DEBUG_WRITTEN_OBJECTS
    written_objects_.clear();
#endif

    format::Marker marker;
    marker.header.size  = sizeof(marker.marker_type) + sizeof(marker.frame_number);
    marker.header.type  = format::kStateMarkerBlock;
    marker.marker_type  = format::kBeginMarker;
    marker.frame_number = frame_number;
    output_stream_->Write(&marker, sizeof(marker));

    // Wait for command queues to complete all pending work.
    WaitForCommandQueues(state_table);

    // Debug objects
    WriteEnableDebugLayer();
    StandardCreateWrite<ID3D12Debug_Wrapper>(state_table);
    StandardCreateWrite<ID3D12Debug1_Wrapper>(state_table);
    WriteEnableDRED();
    StandardCreateWrite<ID3D12DeviceRemovedExtendedDataSettings_Wrapper>(state_table);

    // DXGI objects
    StandardCreateWrite<IDXGIFactory_Wrapper>(state_table);
    StandardCreateWrite<IDXGISurface_Wrapper>(state_table);
    StandardCreateWrite<IDXGIFactoryMedia_Wrapper>(state_table);
    StandardCreateWrite<IDXGIDecodeSwapChain_Wrapper>(state_table);
    StandardCreateWrite<IDXGIAdapter_Wrapper>(state_table);
    StandardCreateWrite<IDXGIDevice_Wrapper>(state_table);
    StandardCreateWrite<IDXGIDisplayControl_Wrapper>(state_table);
    StandardCreateWrite<IDXGIKeyedMutex_Wrapper>(state_table);
    StandardCreateWrite<IDXGIOutput_Wrapper>(state_table);
    StandardCreateWrite<IDXGIOutputDuplication_Wrapper>(state_table);
    StandardCreateWrite<IDXGIResource_Wrapper>(state_table);

#ifdef GFXRECON_AGS_SUPPORT
    // AGS calls
    WriteAgsInitialize(ags_state_table);
    WriteAgsDriverExtensionsDX12CreateDevice(ags_state_table);
#endif // GFXRECON_AGS_SUPPORT

    // Device
    StandardCreateWrite<ID3D12Device_Wrapper>(state_table);

    // Agility SDK constructs
    StandardCreateWrite<ID3D12DeviceFactory_Wrapper>(state_table);
    StandardCreateWrite<ID3D12DeviceConfiguration_Wrapper>(state_table);

    // Write this out before rendering begins
    // This ensures the replayer gets a chance to process the metadata command
    D3D12CaptureManager::Get()->WriteDxgiAdapterInfo();
    D3D12CaptureManager::Get()->WriteDx12DriverInfo();
    D3D12CaptureManager::Get()->WriteDx12RuntimeInfo();

    // Queue
    StandardCreateWrite<ID3D12CommandQueue_Wrapper>(state_table);

    // Swap chain
    WriteSwapChainState(state_table);
    StandardCreateWrite<IDXGISwapChainMedia_Wrapper>(state_table);
    StandardCreateWrite<ID3D12SwapChainAssistant_Wrapper>(state_table);

    // Fences
    WriteFenceState(state_table);

    // Heaps
    StandardCreateWrite<ID3D10Blob_Wrapper>(state_table);
    WriteHeapState(state_table);

    // Root signatures
    StandardCreateWrite<ID3D12RootSignature_Wrapper>(state_table);
    StandardCreateWrite<ID3D12RootSignatureDeserializer_Wrapper>(state_table);
    StandardCreateWrite<ID3D12VersionedRootSignatureDeserializer_Wrapper>(state_table);

    // State objects
    WriteStateObjectsState(state_table);
    WriteStateObjectPropertiesState(state_table);

    // Resources and descriptors
    std::unordered_map<format::HandleId, std::vector<ResourceSnapshotInfo>> resource_snapshots;
    std::unordered_map<format::HandleId, uint64_t>                          max_resource_sizes;
    WriteResourceCreationState(state_table, resource_snapshots, max_resource_sizes);
    WriteDescriptorState(state_table);

    // The resource snapshots must be written after the descriptors in order to support resource value mapping for
    // optimized DXR replay.
    WriteResourceSnapshots(resource_snapshots, max_resource_sizes);

    // Other
    StandardCreateWrite<ID3D12DeviceRemovedExtendedData_Wrapper>(state_table);
    StandardCreateWrite<ID3D12LifetimeOwner_Wrapper>(state_table);
    StandardCreateWrite<ID3D12LifetimeTracker_Wrapper>(state_table);
    StandardCreateWrite<ID3D12MetaCommand_Wrapper>(state_table);
    StandardCreateWrite<ID3D12ProtectedResourceSession_Wrapper>(state_table);
    StandardCreateWrite<ID3D12QueryHeap_Wrapper>(state_table);
    StandardCreateWrite<ID3D12Tools_Wrapper>(state_table);
    StandardCreateWrite<ID3DDestructionNotifier_Wrapper>(state_table);

    // Pipelines
    StandardCreateWrite<ID3D12PipelineLibrary_Wrapper>(state_table);
    StandardCreateWrite<ID3D12PipelineState_Wrapper>(state_table);

    // Debug objects
    StandardCreateWrite<ID3D12Debug2_Wrapper>(state_table);
    StandardCreateWrite<ID3D12DebugDevice1_Wrapper>(state_table);
    StandardCreateWrite<ID3D12DebugDevice_Wrapper>(state_table);
    StandardCreateWrite<ID3D12DebugCommandQueue_Wrapper>(state_table);
    StandardCreateWrite<ID3D12DebugCommandList1_Wrapper>(state_table);
    StandardCreateWrite<ID3D12DebugCommandList_Wrapper>(state_table);
    StandardCreateWrite<ID3D12SharingContract_Wrapper>(state_table);
    StandardCreateWrite<ID3D12InfoQueue_Wrapper>(state_table);

    // Acceleration structures
    WriteAccelerationStructuresState(state_table);

    // Command lists
    StandardCreateWrite<ID3D12CommandAllocator_Wrapper>(state_table);
    StandardCreateWrite<ID3D12CommandSignature_Wrapper>(state_table);
    WriteResidencyPriority(state_table);
    WriteCommandListState(state_table);

    // TODO: Determine dependencies for creation of ID3D12VirtualizationGuestDevice
    // Since the dependency chain is unclear, just write their state at the very end
    StandardCreateWrite<ID3D12VirtualizationGuestDevice_Wrapper>(state_table);

    marker.marker_type = format::kEndMarker;
    output_stream_->Write(&marker, sizeof(marker));

    saved_state_ = nullptr;
}

void Dx12StateWriter::WriteHeapState(const Dx12StateTable& state_table)
{
    std::set<util::MemoryOutputStream*> processed;
    state_table.VisitWrappers([&](format::HandleId id, const ID3D12Heap_Wrapper* wrapper) {
        assert(wrapper != nullptr);
        assert(wrapper->GetObjectInfo() != nullptr);
        assert(wrapper->GetObjectInfo()->create_parameters != nullptr);

        auto wrapper_info = wrapper->GetObjectInfo();
        if (wrapper_info->open_existing_address != nullptr)
        {
            if (!WriteCreateHeapAllocationCmd(wrapper_info->open_existing_address))
            {
                GFXRECON_LOG_ERROR("Failed to retrieve memory information for address specified to "
                                   "ID3D12Device3::OpenExistingHeapFromAddress (error = %d)",
                                   GetLastError());
            }
        }

        Dx12StateWriterBase::StandardCreateWrite(wrapper);
    });
}

void Dx12StateWriter::WriteDescriptorState(const Dx12StateTable& state_table)
{
    std::set<util::MemoryOutputStream*> processed;
    state_table.VisitWrappers([&](format::HandleId id, ID3D12DescriptorHeap_Wrapper* heap_wrapper) {
        assert(heap_wrapper != nullptr);
        assert(heap_wrapper->GetWrappedObject() != nullptr);
        assert(heap_wrapper->GetObjectInfo() != nullptr);
        assert(heap_wrapper->GetObjectInfo()->create_parameters != nullptr);

        auto        heap      = heap_wrapper->GetWrappedObjectAs<ID3D12DescriptorHeap>();
        auto        heap_info = heap_wrapper->GetObjectInfo();
        const auto& heap_desc = heap->GetDesc();

        // Write heap creation call.
        Dx12StateWriterBase::StandardCreateWrite(heap_wrapper);

        // Write GetCPUDescriptorHandleForHeapStart call.
        if (heap_info->cpu_start != 0)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE cpu_start;
            cpu_start.ptr = heap_info->cpu_start;
            EncodeStruct(&encoder_, cpu_start);
            WriteMethodCall(format::ApiCallId::ApiCall_ID3D12DescriptorHeap_GetCPUDescriptorHandleForHeapStart,
                            heap_wrapper->GetCaptureId(),
                            &parameter_stream_);
            parameter_stream_.Reset();
        }

        // Write GetGPUDescriptorHandleForHeapStart call.
        if (heap_info->gpu_start != 0)
        {
            D3D12_GPU_DESCRIPTOR_HANDLE gpu_start;
            gpu_start.ptr = heap_info->gpu_start;
            EncodeStruct(&encoder_, gpu_start);
            WriteMethodCall(format::ApiCallId::ApiCall_ID3D12DescriptorHeap_GetGPUDescriptorHandleForHeapStart,
                            heap_wrapper->GetCaptureId(),
                            &parameter_stream_);
            parameter_stream_.Reset();
        }

        // Write call to query the device for heap increment size.
        encoder_.EncodeEnumValue(heap_desc.Type);
        encoder_.EncodeUInt32Value(heap_info->descriptor_increment);
        WriteMethodCall(format::ApiCallId::ApiCall_ID3D12Device_GetDescriptorHandleIncrementSize,
                        heap_info->create_object_id,
                        &parameter_stream_);
        parameter_stream_.Reset();

        // Write descriptor creation calls, not use StandardCreateWrite.
        for (uint32_t i = 0; i < heap_desc.NumDescriptors; ++i)
        {
            const DxDescriptorInfo& descriptor_info = heap_info->descriptor_info[i];

            GFXRECON_ASSERT(descriptor_info.heap_id == heap_wrapper->GetCaptureId());
            GFXRECON_ASSERT(descriptor_info.index == i);

            if ((CheckDescriptorObjects(descriptor_info, state_table)) &&
                (descriptor_info.create_parameters != nullptr) &&
                (descriptor_info.create_parameters->GetDataSize() != 0))
            {
                if (descriptor_info.is_copy)
                {
                    // Append heap id and descriptor index if the create parameters were copied from another descriptor
                    // in CopyDescriptors.
                    auto dest_heap_id = descriptor_info.heap_id;
                    auto dest_index   = descriptor_info.index;
                    parameter_stream_.Write(descriptor_info.create_parameters->GetData(),
                                            descriptor_info.create_parameters->GetDataSize());
                    parameter_stream_.Write(&dest_heap_id, sizeof(dest_heap_id));
                    parameter_stream_.Write(&dest_index, sizeof(dest_index));
                    WriteMethodCall(
                        descriptor_info.create_call_id, descriptor_info.create_object_id, &parameter_stream_);
                    parameter_stream_.Reset();
                }
                else
                {
                    WriteMethodCall(descriptor_info.create_call_id,
                                    descriptor_info.create_object_id,
                                    descriptor_info.create_parameters.get());
                }
            }
        }
    });
}

void Dx12StateWriter::WriteResourceCreationState(
    const Dx12StateTable&                                                    state_table,
    std::unordered_map<format::HandleId, std::vector<ResourceSnapshotInfo>>& resource_snapshots,
    std::unordered_map<format::HandleId, uint64_t>&                          max_resource_sizes)
{
    HRESULT result = E_FAIL;

    std::vector<MappedSubresourceInfo> mapped_subresources;

    resource_snapshots.clear();
    max_resource_sizes.clear();

    state_table.VisitWrappers([&](format::HandleId id, ID3D12Resource_Wrapper* resource_wrapper) {
        assert(resource_wrapper != nullptr);
        assert(resource_wrapper->GetWrappedObject() != nullptr);
        assert(resource_wrapper->GetObjectInfo() != nullptr);
        assert(resource_wrapper->GetObjectInfo()->create_parameters != nullptr);

        WriteResourceCreation(state_table,
                              resource_wrapper->GetCaptureId(),
                              resource_wrapper->GetObjectInfo(),
                              resource_wrapper->GetWrappedObjectAs<ID3D12Resource>(),
                              resource_wrapper->GetRefCount(),
                              resource_snapshots,
                              mapped_subresources,
                              max_resource_sizes);

        if (saved_state_ != nullptr)
        {
            saved_state_->SaveObjectState(
                resource_wrapper->GetCaptureId(), resource_wrapper->GetRefCount(), resource_wrapper->GetObjectInfo());
        }
    });

    WriteResourceMappings(mapped_subresources);
}

void Dx12StateWriter::WriteFenceState(const Dx12StateTable& state_table)
{
    state_table.VisitWrappers([&](format::HandleId id, ID3D12Fence_Wrapper* fence_wrapper) {
        assert(fence_wrapper != nullptr);
        assert(fence_wrapper->GetWrappedObject() != nullptr);
        assert(fence_wrapper->GetObjectInfo() != nullptr);

        auto fence      = fence_wrapper->GetWrappedObjectAs<ID3D12Fence>();
        auto fence_info = fence_wrapper->GetObjectInfo();

        assert(fence_info->create_parameters != nullptr);
        assert(fence_info->create_object_id != format::kNullHandleId);

        // Write call to create the fence.
        Dx12StateWriterBase::StandardCreateWrite(fence_wrapper);

        UINT64 completed_fence_value = fence->GetCompletedValue();

        // Write SetEventOnCompletion commands for remaining pending events.
        // The pending_events_mutex doesn't need to be locked here because all other threads are blocked while state is
        // being written.
        auto& pending_events = fence_info->pending_events;
        for (auto events : pending_events)
        {
            UINT64 value = events.first;

            // Ignore any events that have already been signaled.
            if (value <= completed_fence_value)
            {
                continue;
            }

            for (auto event : events.second)
            {
                encoder_.EncodeUInt64Value(value);
                encoder_.EncodeVoidPtr(event);
                encoder_.EncodeInt32Value(S_OK);
                WriteMethodCall(format::ApiCallId::ApiCall_ID3D12Fence_SetEventOnCompletion,
                                fence_wrapper->GetCaptureId(),
                                &parameter_stream_);
                parameter_stream_.Reset();
            }
        }

        // Write call to signal the fence to its most recent value.
        encoder_.EncodeUInt64Value(completed_fence_value);
        encoder_.EncodeInt32Value(S_OK);
        WriteMethodCall(
            format::ApiCallId::ApiCall_ID3D12Fence_Signal, fence_wrapper->GetCaptureId(), &parameter_stream_);
        parameter_stream_.Reset();

        if (saved_state_)
        {
            saved_state_->SaveFenceState(
                fence_wrapper->GetCaptureId(), fence_wrapper->GetRefCount(), fence_info, completed_fence_value);
        }
    });
}

void Dx12StateWriter::WriteResidencyPriority(const Dx12StateTable& state_table)
{
    state_table.VisitWrappers([&](format::HandleId id, ID3D12Device_Wrapper* device_wrapper) {
        GFXRECON_ASSERT(device_wrapper != nullptr);
        GFXRECON_ASSERT(device_wrapper->GetObjectInfo() != nullptr);

        auto     device_info = device_wrapper->GetObjectInfo();
        uint32_t num_objects = static_cast<uint32_t>(device_info->residency_priorities.size());

        if (num_objects > 0)
        {
            auto handle_id = device_wrapper->GetCaptureId();

            std::vector<format::HandleId>         handles;
            std::vector<D3D12_RESIDENCY_PRIORITY> priorities;

            for (auto& entry : device_info->residency_priorities)
            {
                handles.emplace_back(entry.first);
                priorities.emplace_back(entry.second);
            }

            encoder_.EncodeUInt32Value(num_objects);
            encoder_.EncodeHandleIdArray(handles.data(), num_objects);
            encoder_.EncodeEnumArray(priorities.data(), num_objects);
            encoder_.EncodeInt32Value(S_OK);
            WriteMethodCall(
                format::ApiCallId::ApiCall_ID3D12Device1_SetResidencyPriority, handle_id, &parameter_stream_);
            parameter_stream_.Reset();
        }
    });
}

void Dx12StateWriter::WriteCommandListState(const Dx12StateTable& state_table)
{
    std::vector<CreateInfo<ID3D12CommandListInfo>> bundle_command_lists;
    std::vector<CreateInfo<ID3D12CommandListInfo>> direct_command_lists;

    state_table.VisitWrappers([&](format::HandleId id, ID3D12CommandList_Wrapper* list_wrapper) {
        GFXRECON_ASSERT(list_wrapper != nullptr);
        GFXRECON_ASSERT(list_wrapper->GetWrappedObject() != nullptr);
        GFXRECON_ASSERT(list_wrapper->GetObjectInfo() != nullptr);

        auto list      = list_wrapper->GetWrappedObjectAs<ID3D12CommandList>();
        auto list_info = list_wrapper->GetObjectInfo();

        GFXRECON_ASSERT(list_info->create_parameters != nullptr);
        GFXRECON_ASSERT(list_info->create_object_id != format::kNullHandleId);

        // Keep track of bundled and direct command lists.
        if (list->GetType() == D3D12_COMMAND_LIST_TYPE_BUNDLE)
        {
            bundle_command_lists.push_back({ id, list_info.get(), list_wrapper->GetRefCount() });
        }
        else
        {
            direct_command_lists.push_back({ id, list_info.get(), list_wrapper->GetRefCount() });
        }

        if (saved_state_)
        {
            saved_state_->SaveCommandListState(id, list_info, list_wrapper->GetRefCount(), list->GetType());
        }
    });

    Dx12StateWriterBase::WriteCommandListState(bundle_command_lists, direct_command_lists, state_table);
}

void Dx12StateWriter::WriteSwapChainState(const Dx12StateTable& state_table)
{
    state_table.VisitWrappers([&](format::HandleId id, IDXGISwapChain_Wrapper* swapchain_wrapper) {
        GFXRECON_ASSERT(swapchain_wrapper != nullptr);
        GFXRECON_ASSERT(swapchain_wrapper->GetWrappedObject() != nullptr);
        GFXRECON_ASSERT(swapchain_wrapper->GetObjectInfo() != nullptr);

        auto swapchain      = swapchain_wrapper->GetWrappedObjectAs<IDXGISwapChain>();
        auto swapchain_info = swapchain_wrapper->GetObjectInfo();

        // Write swapchain creation call.
        Dx12StateWriterBase::StandardCreateWrite(swapchain_wrapper);

        // Write call to resize the swapchain buffers.
        if (swapchain_info->resize_info.call_id != format::ApiCall_Unknown)
        {
            WriteMethodCall(swapchain_info->resize_info.call_id,
                            swapchain_wrapper->GetCaptureId(),
                            swapchain_info->resize_info.call_parameters.get());
        }

        // Write image state command.
        UINT                                  swapchain_buffer_index = 0;
        graphics::dx12::IDXGISwapChain3ComPtr swapchain3;
        if (SUCCEEDED(swapchain->QueryInterface(IID_PPV_ARGS(&swapchain3))))
        {
            swapchain_buffer_index = swapchain3->GetCurrentBackBufferIndex();
        }
        else
        {
            GFXRECON_LOG_ERROR("Failed to get current swap chain (id=%" PRIu64
                               ") buffer index. Swap chain may not replay correctly.",
                               swapchain_wrapper->GetCaptureId());
        }

        format::SetSwapchainImageStateCommandHeader header;

        // Initialize standard block header.
        header.meta_header.block_header.size = format::GetMetaDataBlockBaseSize(header);
        header.meta_header.block_header.type = format::kMetaDataBlock;

        // Initialize block data for set-swapchain-image-state meta-data command.
        header.meta_header.meta_data_id = format::MakeMetaDataId(format::ApiFamilyId::ApiFamily_D3D12,
                                                                 format::MetaDataType::kSetSwapchainImageStateCommand);
        header.thread_id                = thread_id_;
        header.swapchain_id             = swapchain_wrapper->GetCaptureId();

        // The object used to create a swap chain is a command queue for DX12. Store the command_queue's HandleId in
        // the header's device_id field.
        header.device_id = swapchain_info->command_queue_id;

        // last_presented_image is used to store the current swapchain buffer index for DX12.
        header.last_presented_image = swapchain_buffer_index;

        // last_presented_image is the only need for replay, so nothing is written into format::SwapchainImageStateInfo.
        header.image_info_count = 0;

        output_stream_->Write(&header, sizeof(header));

        if (saved_state_)
        {
            saved_state_->SaveSwapChainState(swapchain_wrapper->GetCaptureId(),
                                             swapchain_wrapper->GetRefCount(),
                                             swapchain_info,
                                             swapchain_buffer_index);
        }
    });
}

void Dx12StateWriter::WriteEnableDebugLayer()
{
    auto debug_object_id = D3D12CaptureManager::Get()->GetEnableDebugLayerObjectId();
    if (debug_object_id != format::kNullHandleId)
    {
        EncodeStruct(&encoder_, IID_ID3D12Debug);
        encoder_.EncodeHandleIdPtr(&debug_object_id);
        encoder_.EncodeUInt32Value(S_OK);
        WriteFunctionCall(format::ApiCallId::ApiCall_D3D12GetDebugInterface, &parameter_stream_);
        parameter_stream_.Reset();
        WriteMethodCall(format::ApiCallId::ApiCall_ID3D12Debug_EnableDebugLayer, debug_object_id, &parameter_stream_);
        WriteMethodCall(format::ApiCallId::ApiCall_IUnknown_Release, debug_object_id, &parameter_stream_);
    }
}

void Dx12StateWriter::WriteEnableDRED()
{
    const auto& enable_dred_info = D3D12CaptureManager::Get()->GetEnableDREDInfo();
    if (enable_dred_info.dred_settings1_object_id != format::kNullHandleId)
    {
        EncodeStruct(&encoder_, IID_ID3D12DeviceRemovedExtendedDataSettings1);
        encoder_.EncodeHandleIdPtr(&enable_dred_info.dred_settings1_object_id);
        encoder_.EncodeUInt32Value(S_OK);
        WriteFunctionCall(format::ApiCallId::ApiCall_D3D12GetDebugInterface, &parameter_stream_);
        parameter_stream_.Reset();

        encoder_.EncodeEnumValue(enable_dred_info.set_auto_breadcrumbs_enablement_);
        WriteMethodCall(format::ApiCallId::ApiCall_ID3D12DeviceRemovedExtendedDataSettings_SetAutoBreadcrumbsEnablement,
                        enable_dred_info.dred_settings1_object_id,
                        &parameter_stream_);
        parameter_stream_.Reset();

        encoder_.EncodeEnumValue(enable_dred_info.set_breadcrumb_context_enablement_);
        WriteMethodCall(
            format::ApiCallId::ApiCall_ID3D12DeviceRemovedExtendedDataSettings1_SetBreadcrumbContextEnablement,
            enable_dred_info.dred_settings1_object_id,
            &parameter_stream_);
        parameter_stream_.Reset();

        encoder_.EncodeEnumValue(enable_dred_info.set_page_fault_enablement_);
        WriteMethodCall(format::ApiCallId::ApiCall_ID3D12DeviceRemovedExtendedDataSettings_SetPageFaultEnablement,
                        enable_dred_info.dred_settings1_object_id,
                        &parameter_stream_);
        parameter_stream_.Reset();

        WriteMethodCall(
            format::ApiCallId::ApiCall_IUnknown_Release, enable_dred_info.dred_settings1_object_id, &parameter_stream_);
    }
}

void Dx12StateWriter::WriteAccelerationStructuresState(const Dx12StateTable& state_table)
{
    std::map<uint64_t, const DxAccelerationStructureBuildInfo*> build_infos;

    // Find all acceleration structures that exist on resources.
    state_table.VisitWrappers([&](format::HandleId id, ID3D12Resource_Wrapper* resource_wrapper) {
        GFXRECON_ASSERT(resource_wrapper != nullptr);
        GFXRECON_ASSERT(resource_wrapper->GetObjectInfo() != nullptr);

        const auto resource_info = resource_wrapper->GetObjectInfo();

        for (const auto& pair : resource_info->acceleration_structure_builds)
        {
            GFXRECON_ASSERT(build_infos.count(pair.second.id) == 0);
            build_infos[pair.second.id] = &pair.second;
        }
    });

    WriteAccelerationStructuresState(build_infos);
}

void Dx12StateWriter::WriteAccelerationStructuresState(
    std::map<uint64_t, const DxAccelerationStructureBuildInfo*> as_builds)
{
    uint64_t                            accel_struct_file_bytes = 0;
    std::set<D3D12_GPU_VIRTUAL_ADDRESS> blas_addresses;
    std::set<D3D12_GPU_VIRTUAL_ADDRESS> written_addresses;

    // Write accel struct info.
    for (const auto& as_build_pair : as_builds)
    {
        const auto& as_build = *as_build_pair.second;

        GFXRECON_ASSERT(as_build.id != 0);

        // The destination resource must still exist.
        GFXRECON_ASSERT(CheckGpuVa(as_build.dest_gpu_va));

        if (as_build.input_data_resource == nullptr)
        {
            GFXRECON_LOG_ERROR(
                "Found an unexpected null input data resource when writing acceleration structure (id=%" PRIu64
                "). The acceleration structure will not be written to the trim state.",
                as_build.id);
            continue;
        }

        uint8_t* inputs_data_ptr = nullptr;
        HRESULT  hr = graphics::dx12::MapSubresource(as_build.input_data_resource, 0, nullptr, inputs_data_ptr);
        if (SUCCEEDED(hr) && (inputs_data_ptr != nullptr))
        {
            // Check that the instance desc addresses are correct for TLAS.
            if (as_build.inputs.Type == D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL)
            {
                bool           valid          = true;
                constexpr auto address_stride = sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
                constexpr auto address_offset = offsetof(D3D12_RAYTRACING_INSTANCE_DESC, AccelerationStructure);
                for (UINT i = 0; i < as_build.inputs.NumDescs; ++i)
                {
                    D3D12_GPU_VIRTUAL_ADDRESS* address = reinterpret_cast<D3D12_GPU_VIRTUAL_ADDRESS*>(
                        inputs_data_ptr + i * address_stride + address_offset);
                    if (blas_addresses.count(*address) == 0)
                    {
                        valid = false;
                        break;
                    }
                }
                if (!valid)
                {
                    continue;
                }
            }
        }
        else
        {
            GFXRECON_LOG_FATAL("Failed to map inputs data resource for writing trimmed state for acceleration "
                               "structures. The capture file will be invalid.");
            continue;
        }

        bool     is_copy            = false;
        bool     is_in_place_copy   = false;
        uint64_t copy_source_gpu_va = 0;
        if (as_build.copy_source_gpu_va != 0)
        {
            is_copy = true;
            if (written_addresses.count(as_build.copy_source_gpu_va) == 0)
            {
                is_in_place_copy   = true;
                copy_source_gpu_va = as_build.dest_gpu_va;
            }
            else
            {
                is_in_place_copy   = false;
                copy_source_gpu_va = as_build.copy_source_gpu_va;
            }
        }

        bool write_build_data = (!is_copy || is_in_place_copy);

        // Write header.
        format::InitDx12AccelerationStructureCommandHeader cmd;
        cmd.meta_header.meta_data_id = format::MakeMetaDataId(
            format::ApiFamilyId::ApiFamily_D3D12, format::MetaDataType::kInitDx12AccelerationStructureCommand);
        cmd.meta_header.block_header.type    = format::BlockType::kMetaDataBlock;
        cmd.thread_id                        = thread_id_;
        cmd.dest_acceleration_structure_data = as_build.dest_gpu_va;
        cmd.copy_source_gpu_va               = copy_source_gpu_va;
        cmd.copy_mode                        = is_copy ? as_build.copy_mode : 0;
        cmd.inputs_type                      = as_build.inputs.Type;

        // Remove PERFORM_UPDATE flag
        auto build_flags = as_build.inputs.Flags;
        if ((build_flags & D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE) ==
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE)
        {
            build_flags &= ~D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
            build_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
        }
        cmd.inputs_flags = build_flags;

        // Get NumDescs and data sizes.
        size_t inputs_data_ptr_file_size = 0;
        cmd.inputs_data_size             = 0;
        cmd.inputs_num_instance_descs    = 0;
        cmd.inputs_num_geometry_descs    = 0;
        if (write_build_data)
        {
            cmd.inputs_data_size = as_build.input_data_size;
            if (as_build.inputs.Type == D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL)
            {
                cmd.inputs_num_geometry_descs = as_build.inputs.NumDescs;
            }
            else if (as_build.inputs.Type == D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL)
            {
                cmd.inputs_num_instance_descs = as_build.inputs.NumDescs;
            }
            else
            {
                GFXRECON_ASSERT(false && "Invalid D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE.");
            }

            GFXRECON_CHECK_CONVERSION_DATA_LOSS(size_t, as_build.input_data_size);
            inputs_data_ptr_file_size = static_cast<size_t>(as_build.input_data_size);
            if (compressor_ != nullptr)
            {
                // Compress block data.
                size_t compressed_size =
                    compressor_->Compress(inputs_data_ptr_file_size, inputs_data_ptr, &compressed_parameter_buffer_, 0);

                if ((compressed_size > 0) && (compressed_size < as_build.input_data_size))
                {
                    cmd.meta_header.block_header.type = format::BlockType::kCompressedMetaDataBlock;

                    inputs_data_ptr           = compressed_parameter_buffer_.data();
                    inputs_data_ptr_file_size = compressed_size;
                }
            }
        }

        // Compute file block size and write header to file.
        cmd.meta_header.block_header.size =
            format::GetMetaDataBlockBaseSize(cmd) +
            (sizeof(format::InitDx12AccelerationStructureGeometryDesc) * cmd.inputs_num_geometry_descs) +
            inputs_data_ptr_file_size;
        output_stream_->Write(&cmd, sizeof(cmd));
        accel_struct_file_bytes += sizeof(cmd);

        // Write geometry and inputs data to file.
        if (write_build_data)
        {
            // Write geometry descs for BLAS.
            if (as_build.inputs.Type == D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL)
            {
                GFXRECON_ASSERT(as_build.inputs.NumDescs == as_build.inputs_geometry_descs.size());

                for (const auto& geom : as_build.inputs_geometry_descs)
                {
                    format::InitDx12AccelerationStructureGeometryDesc geom_info;
                    geom_info.geometry_type  = geom.Type;
                    geom_info.geometry_flags = geom.Flags;
                    if (geom.Type == D3D12_RAYTRACING_GEOMETRY_TYPE_PROCEDURAL_PRIMITIVE_AABBS)
                    {
                        geom_info.aabbs_count  = geom.AABBs.AABBCount;
                        geom_info.aabbs_stride = geom.AABBs.AABBs.StrideInBytes;
                    }
                    else if (geom.Type == D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES)
                    {
                        geom_info.triangles_has_transform = (geom.Triangles.Transform3x4 == 0) ? 0 : 1;
                        geom_info.triangles_index_format  = geom.Triangles.IndexFormat;
                        geom_info.triangles_vertex_format = geom.Triangles.VertexFormat;
                        geom_info.triangles_index_count =
                            (geom.Triangles.IndexBuffer == 0) ? 0 : geom.Triangles.IndexCount;
                        geom_info.triangles_vertex_count  = geom.Triangles.VertexCount;
                        geom_info.triangles_vertex_stride = geom.Triangles.VertexBuffer.StrideInBytes;
                    }
                    else
                    {
                        GFXRECON_ASSERT(false && "Invalid D3D12_RAYTRACING_GEOMETRY_TYPE.");
                    }
                    output_stream_->Write(&geom_info, sizeof(geom_info));
                    accel_struct_file_bytes += sizeof(geom_info);
                }
            }

            // Write inputs data.
            output_stream_->Write(inputs_data_ptr, inputs_data_ptr_file_size);
            accel_struct_file_bytes += inputs_data_ptr_file_size;
            as_build.input_data_resource->Unmap(0, nullptr);
        }

        // Track which accel struct addresses have been written to the trim state block.
        if (as_build.inputs.Type == D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL)
        {
            blas_addresses.insert(as_build.dest_gpu_va);
        }
        written_addresses.insert(as_build.dest_gpu_va);
    }

    if (accel_struct_file_bytes > 0)
    {
        GFXRECON_LOG_INFO("Wrote acceleration structure build data to trim state block: %" PRIu64
                          " bottom level, %" PRIu64 " top level, %" PRIu64 " file bytes.",
                          blas_addresses.size(),
                          (written_addresses.size() - blas_addresses.size()),
                          accel_struct_file_bytes);
    }
}

void Dx12StateWriter::WriteStateObjectAndDependency(const format::HandleId                state_object_id,
                                                    const ID3D12StateObjectInfo*          state_object_info,
                                                    std::unordered_set<format::HandleId>& written_objs,
                                                    unsigned long                         ref_count)
{
    if (written_objs.count(state_object_id) == 0)
    {
        for (const auto& root_sig_info_pair : state_object_info->root_signature_wrapper_infos)
        {
            auto root_sig_id = root_sig_info_pair.first;
            if ((root_sig_info_pair.second->GetWrapper() == nullptr) && (written_objs.count(root_sig_id) == 0))
            {
                Dx12StateWriterBase::StandardCreateWrite(root_sig_id, root_sig_info_pair.second.get(), 1);
                written_objs.insert(root_sig_id);
            }
        }

        if (state_object_info->grow_from_state_object_wrapper_info.first != gfxrecon::format::kNullHandleId)
        {
            auto grow_from_state_obj_id = state_object_info->grow_from_state_object_wrapper_info.first;
            if (written_objs.count(grow_from_state_obj_id) == 0)
            {
                WriteStateObjectAndDependency(grow_from_state_obj_id,
                                              static_cast<const ID3D12StateObjectInfo*>(
                                                  state_object_info->grow_from_state_object_wrapper_info.second.get()),
                                              written_objs,
                                              1);
            }
        }
        Dx12StateWriterBase::StandardCreateWrite(state_object_id, state_object_info, ref_count);
        written_objs.insert(state_object_id);
    }
}

void Dx12StateWriter::WriteStateObjectsState(const Dx12StateTable& state_table)
{
    std::unordered_set<format::HandleId> written_objs;
    state_table.VisitWrappers([&](format::HandleId id, const ID3D12StateObject_Wrapper* state_object_wrapper) {
        GFXRECON_ASSERT(state_object_wrapper != nullptr);
        GFXRECON_ASSERT(state_object_wrapper->GetObjectInfo() != nullptr);
        GFXRECON_ASSERT(state_object_wrapper->GetObjectInfo()->create_parameters != nullptr);

        WriteStateObjectAndDependency(state_object_wrapper->GetCaptureId(),
                                      state_object_wrapper->GetObjectInfo().get(),
                                      written_objs,
                                      state_object_wrapper->GetRefCount());
    });
}

void Dx12StateWriter::WriteStateObjectPropertiesState(const Dx12StateTable& state_table)
{
    state_table.VisitWrappers([&](format::HandleId id, const ID3D12StateObjectProperties_Wrapper* wrapper) {
        GFXRECON_ASSERT(wrapper != nullptr);
        GFXRECON_ASSERT(wrapper->GetObjectInfo() != nullptr);

        auto wrapper_info = wrapper->GetObjectInfo();
        Dx12StateWriterBase::StandardCreateWrite(wrapper->GetCaptureId(), wrapper_info.get(), 1);

        // Write GetShaderIdentifier calls before potentially releasing the reference to the
        // ID3D12StateObjectProperties.
        const auto& call_parameters_map = wrapper->GetObjectInfo()->get_shader_identifier_call_parameters;
        for (auto& call_parameters_pair : call_parameters_map)
        {
            WriteMethodCall(format::ApiCall_ID3D12StateObjectProperties_GetShaderIdentifier,
                            wrapper->GetCaptureId(),
                            call_parameters_pair.second.get());
        }

        WriteAddRefAndReleaseCommands(wrapper->GetCaptureId(), 1, wrapper->GetRefCount());
    });
}

#ifdef GFXRECON_AGS_SUPPORT
void Dx12StateWriter::WriteAgsInitialize(const AgsStateTable& ags_state_table)
{
    util::MemoryOutputStream* create_data = ags_state_table.GetAgsContextCreateData();

    if (create_data != nullptr)
    {
        WriteFunctionCall(format::ApiCallId::ApiCall_Ags_agsInitialize_6_0_1, create_data);
    }
}

void Dx12StateWriter::WriteAgsDriverExtensionsDX12CreateDevice(const AgsStateTable& ags_state_table)
{
    util::MemoryOutputStream* create_data = ags_state_table.GetAgsDeviceCreateData();

    if (create_data != nullptr)
    {
        WriteFunctionCall(format::ApiCallId::ApiCall_Ags_agsDriverExtensionsDX12_CreateDevice_6_0_1, create_data);
    }
}
#endif // GFXRECON_AGS_SUPPORT

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)
