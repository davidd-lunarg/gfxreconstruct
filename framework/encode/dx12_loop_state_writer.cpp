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

#include "encode/dx12_loop_state_writer.h"

#include "generated/generated_dx12_api_call_encoders.h"
#include "encode/custom_dx12_struct_encoders.h"
#include "encode/custom_dx12_struct_unwrappers.h"
#include "encode/struct_pointer_encoder.h"
#include "encode/d3d12_capture_manager.h"
#include "graphics/dx12_resource_data_util.h"
#include "graphics/dx12_util.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

Dx12LoopStateWriter::Dx12LoopStateWriter(util::FileOutputStream* output_stream,
                                         util::Compressor*       compressor,
                                         format::ThreadId        thread_id) :
    Dx12StateWriterBase(output_stream, compressor, thread_id),
    saved_state_(nullptr)
{
    assert(output_stream != nullptr);
}

Dx12LoopStateWriter::~Dx12LoopStateWriter() {}

#ifdef GFXRECON_AGS_SUPPORT
void Dx12LoopStateWriter::WriteState(const Dx12StateTable& state_table,
                                     const AgsStateTable&  ags_state_table,
                                     uint64_t              frame_number,
                                     Dx12SavedState*       saved_state)
#else
void Dx12LoopStateWriter::WriteState(const Dx12StateTable& state_table,
                                     uint64_t              frame_number,
                                     Dx12SavedState*       saved_state)
#endif // GFXRECON_AGS_SUPPORT
{
    saved_state_ = saved_state;

    format::Marker marker;
    marker.header.size  = sizeof(marker.marker_type) + sizeof(marker.frame_number);
    marker.header.type  = format::kLoopStateMarkerBlock;
    marker.marker_type  = format::kBeginMarker;
    marker.frame_number = frame_number;
    output_stream_->Write(&marker, sizeof(marker));

    // Wait for command queues to complete all pending work.
    WaitForCommandQueues(state_table);

    // Debug objects
    // LOOP_TODO: WriteEnableDebugLayer();
    StandardProcessObjectsReset<ID3D12Debug_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12Debug1_Wrapper>(state_table);
    // LOOP_TODO: WriteEnableDRED();
    StandardProcessObjectsReset<ID3D12DeviceRemovedExtendedDataSettings_Wrapper>(state_table);

    // DXGI objects
    StandardProcessObjectsReset<IDXGIFactory_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGISurface_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIFactoryMedia_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIDecodeSwapChain_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIAdapter_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIDevice_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIDisplayControl_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIKeyedMutex_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIOutput_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIOutputDuplication_Wrapper>(state_table);
    StandardProcessObjectsReset<IDXGIResource_Wrapper>(state_table);

#ifdef GFXRECON_AGS_SUPPORT
    // AGS calls
    // LOOP_TODO: WriteAgsInitialize(ags_state_table);
    // LOOP_TODO: WriteAgsDriverExtensionsDX12CreateDevice(ags_state_table);
#endif // GFXRECON_AGS_SUPPORT

    // Device
    StandardProcessObjectsReset<ID3D12Device_Wrapper>(state_table);

    // Agility SDK constructs
    StandardProcessObjectsReset<ID3D12DeviceFactory_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12DeviceConfiguration_Wrapper>(state_table);

    // Write this out before rendering begins
    // This ensures the replayer gets a chance to process the metadata command
    // LOOP_TODO: D3D12CaptureManager::Get()->WriteDxgiAdapterInfo();
    // LOOP_TODO: D3D12CaptureManager::Get()->WriteDx12DriverInfo();
    // LOOP_TODO: D3D12CaptureManager::Get()->WriteDx12RuntimeInfo();

    // Queue
    StandardProcessObjectsReset<ID3D12CommandQueue_Wrapper>(state_table);

    // Swap chain
    WriteSwapChainState(state_table);
    StandardProcessObjectsReset<IDXGISwapChainMedia_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12SwapChainAssistant_Wrapper>(state_table);

    // Fences
    ProcessObjectsReset<ID3D12Fence_Wrapper, Dx12SavedFenceState>(state_table);

    // Heaps
    StandardProcessObjectsReset<ID3D10Blob_Wrapper>(state_table);
    ProcessObjectsReset<ID3D12Heap_Wrapper, Dx12SavedObjectState>(state_table);

    // Root signatures
    StandardProcessObjectsReset<ID3D12RootSignature_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12RootSignatureDeserializer_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12VersionedRootSignatureDeserializer_Wrapper>(state_table);

    // State objects
    // LOOP_TODO: WriteStateObjectsState(state_table);
    // LOOP_TODO: WriteStateObjectPropertiesState(state_table);

    // Resources and descriptors
    // LOOP_TODO: std::unordered_map<format::HandleId, std::vector<ResourceSnapshotInfo>> resource_snapshots;
    // LOOP_TODO: std::unordered_map<format::HandleId, uint64_t>                          max_resource_sizes;
    // LOOP_TODO: WriteResourceCreationState(state_table, resource_snapshots, max_resource_sizes);
    // LOOP_TODO: WriteDescriptorState(state_table);

    // The resource snapshots must be written after the descriptors in order to support resource value mapping for
    // optimized DXR replay.
    // LOOP_TODO: WriteResourceSnapshots(resource_snapshots, max_resource_sizes);

    // Other
    StandardProcessObjectsReset<ID3D12DeviceRemovedExtendedData_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12LifetimeOwner_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12LifetimeTracker_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12MetaCommand_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12ProtectedResourceSession_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12QueryHeap_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12Tools_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3DDestructionNotifier_Wrapper>(state_table);

    // Pipelines
    StandardProcessObjectsReset<ID3D12PipelineLibrary_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12PipelineState_Wrapper>(state_table);

    // Debug objects
    StandardProcessObjectsReset<ID3D12Debug2_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12DebugDevice1_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12DebugDevice_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12DebugCommandQueue_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12DebugCommandList1_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12DebugCommandList_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12SharingContract_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12InfoQueue_Wrapper>(state_table);

    // Acceleration structures
    // LOOP_TODO: WriteAccelerationStructuresState(state_table);

    // Command lists
    StandardProcessObjectsReset<ID3D12CommandAllocator_Wrapper>(state_table);
    StandardProcessObjectsReset<ID3D12CommandSignature_Wrapper>(state_table);
    // LOOP_TODO: WriteResidencyPriority(state_table);
    WriteCommandListState(state_table);

    // TODO: Determine dependencies for creation of ID3D12VirtualizationGuestDevice
    // Since the dependency chain is unclear, just write their state at the very end
    StandardProcessObjectsReset<ID3D12VirtualizationGuestDevice_Wrapper>(state_table);

    marker.marker_type = format::kEndMarker;
    output_stream_->Write(&marker, sizeof(marker));

    saved_state_ = nullptr;
}

template <>
bool Dx12LoopStateWriter::ProcessObjectReset(Dx12ObjectResetInfo<ID3D12Fence_Wrapper, Dx12SavedFenceState>& reset_info)
{
    GFXRECON_LOG_INFO_ONCE("Processing Fence Reset");

    auto id                = reset_info.id;
    auto fence_wrapper     = reset_info.wrapper;
    auto saved_fence_state = reset_info.saved_state;

    bool exists_before = (saved_fence_state == nullptr);
    bool exists_after  = (fence_wrapper == nullptr);

    bool changed = exists_before != exists_after;

    // Signal all fences to allow all pending GPU work to complete.
    if (exists_after)
    {
        auto fence      = fence_wrapper->GetWrappedObjectAs<ID3D12Fence>();
        auto fence_info = fence_wrapper->GetObjectInfo();

        UINT64 current_completed_fence_value = fence->GetCompletedValue();

        UINT64 max_pending_event_signal_value = current_completed_fence_value;
        for (const auto& events : fence_info->pending_events)
        {
            UINT64 event_signal_value = events.first;

            // Ignore any events that have already been signaled.
            if (event_signal_value <= current_completed_fence_value)
            {
                continue;
            }

            max_pending_event_signal_value = std::max(max_pending_event_signal_value, event_signal_value);
        }

        if (max_pending_event_signal_value > current_completed_fence_value)
        {
            // Write call to signal the fence to clear out pending events.
            encoder_.EncodeUInt64Value(max_pending_event_signal_value);
            encoder_.EncodeInt32Value(S_OK);
            WriteMethodCall(
                format::ApiCallId::ApiCall_ID3D12Fence_Signal, fence_wrapper->GetCaptureId(), &parameter_stream_);
            parameter_stream_.Reset();
        }

        // Release the fence if it was created during the loop.
        if (!exists_before)
        {
            WriteAddRefAndReleaseCommands(id, fence_wrapper->GetRefCount(), 0);
        }

        changed = changed || (max_pending_event_signal_value != saved_fence_state->completed_value);
    }

    if (changed && exists_before)
    {
        // Create the fence if it was deleted during the loop.
        if (!exists_after)
        {
            Dx12StateWriterBase::StandardCreateWrite(
                id, saved_fence_state->object_info.get(), saved_fence_state->ref_count);
        }

        // Write call to signal the fence to its saved value.
        encoder_.EncodeUInt64Value(saved_fence_state->completed_value);
        encoder_.EncodeInt32Value(S_OK);
        WriteMethodCall(
            format::ApiCallId::ApiCall_ID3D12Fence_Signal, fence_wrapper->GetCaptureId(), &parameter_stream_);
        parameter_stream_.Reset();
    }

    return changed;
}

void Dx12LoopStateWriter::WriteCommandListState(const Dx12StateTable& state_table)
{
    std::vector<CreateInfo<ID3D12CommandListInfo>> bundle_command_lists;
    std::vector<CreateInfo<ID3D12CommandListInfo>> direct_command_lists;

    saved_state_->VisitObjectsForReset<ID3D12CommandList_Wrapper, Dx12SavedCommandListState>(
        state_table, [&](Dx12ObjectResetInfo<ID3D12CommandList_Wrapper, Dx12SavedCommandListState> reset_info) {
            GFXRECON_LOG_INFO_ONCE("Processing Command List Reset");

            auto id               = reset_info.id;
            auto list_wrapper     = reset_info.wrapper;
            auto saved_list_state = reset_info.saved_state;

            // Release all command lists.
            if (list_wrapper != nullptr)
            {
                WriteAddRefAndReleaseCommands(id, list_wrapper->GetRefCount(), 0);
            }

            if (saved_list_state != nullptr)
            {
                auto list_info = reinterpret_cast<const ID3D12CommandListInfo*>(saved_list_state->object_info.get());
                auto ref_count = saved_list_state->ref_count;

                // Keep track of bundled and direct command lists.
                if (saved_list_state->list_type == D3D12_COMMAND_LIST_TYPE_BUNDLE)
                {
                    bundle_command_lists.push_back({ id, list_info, ref_count });
                }
                else
                {
                    direct_command_lists.push_back({ id, list_info, ref_count });
                }
            }
        });

    Dx12StateWriterBase::WriteCommandListState(bundle_command_lists, direct_command_lists, state_table);
}

void Dx12LoopStateWriter::WriteSwapChainState(const Dx12StateTable& state_table)
{
    saved_state_->VisitObjectsForReset<IDXGISwapChain_Wrapper, Dx12SavedSwapChainState>(
        state_table, [&](Dx12ObjectResetInfo<IDXGISwapChain_Wrapper, Dx12SavedSwapChainState> reset_info) {
            GFXRECON_LOG_INFO_ONCE("Processing Swap Chain Reset");

            auto id                    = reset_info.id;
            auto swapchain_wrapper     = reset_info.wrapper;
            auto saved_swapchain_state = reset_info.saved_state;

            if (saved_swapchain_state == nullptr)
            {
                GFXRECON_ASSERT(swapchain_wrapper != nullptr);

                // If the object doesn't exist in the saved state, release it.
                WriteAddRefAndReleaseCommands(id, swapchain_wrapper->GetRefCount(), 0);
            }
            else
            {
                // If the object only exists in the saved state, recreate it.
                bool     skip_swapchain_resize   = true;
                uint32_t current_swapchain_image = 0;
                if (swapchain_wrapper == nullptr)
                {
                    Dx12StateWriterBase::StandardCreateWrite(
                        id, saved_swapchain_state->object_info.get(), saved_swapchain_state->ref_count);
                }
                // If the object exists in both the saved and current state, match ref count to saved state.
                else
                {
                    GFXRECON_ASSERT(swapchain_wrapper->GetCaptureId() == id);

                    WriteAddRefAndReleaseCommands(
                        id, swapchain_wrapper->GetRefCount(), saved_swapchain_state->ref_count);

                    // Don't resize if the resize command is the same.
                    auto current_swapchain_info = swapchain_wrapper->GetObjectInfo();
                    skip_swapchain_resize =
                        (saved_swapchain_state->resize_info.call_id == format::ApiCall_Unknown) ||
                        ((current_swapchain_info->resize_info.call_id == saved_swapchain_state->resize_info.call_id) &&
                         (current_swapchain_info->resize_info.call_parameters->GetDataSize() ==
                          saved_swapchain_state->resize_info.call_parameters->GetDataSize()) &&
                         (util::platform::MemoryCompare(
                              saved_swapchain_state->resize_info.call_parameters->GetData(),
                              current_swapchain_info->resize_info.call_parameters->GetData(),
                              saved_swapchain_state->resize_info.call_parameters->GetDataSize()) == 0));

                    // Get current swapchain image index.
                    auto swapchain = swapchain_wrapper->GetWrappedObjectAs<IDXGISwapChain>();
                    graphics::dx12::IDXGISwapChain3ComPtr swapchain3;
                    if (SUCCEEDED(swapchain->QueryInterface(IID_PPV_ARGS(&swapchain3))))
                    {
                        current_swapchain_image = swapchain3->GetCurrentBackBufferIndex();
                    }
                    else
                    {
                        GFXRECON_LOG_ERROR("Failed to get current swap chain (id=%" PRIu64
                                           ") buffer index. Swap chain may not loop correctly.",
                                           id);
                    }
                }

                // Apply saved swapchain state.
                if (!skip_swapchain_resize)
                {
                    if (saved_swapchain_state->resize_info.call_id != format::ApiCall_Unknown)
                    {
                        WriteMethodCall(saved_swapchain_state->resize_info.call_id,
                                        id,
                                        saved_swapchain_state->resize_info.call_parameters.get());
                    }
                }

                // Write image state command.
                if (current_swapchain_image != saved_swapchain_state->last_presented_image)
                {
                    format::SetSwapchainImageStateCommandHeader header;

                    // Initialize standard block header.
                    header.meta_header.block_header.size = format::GetMetaDataBlockBaseSize(header);
                    header.meta_header.block_header.type = format::kMetaDataBlock;

                    // Initialize block data for set-swapchain-image-state meta-data command.
                    header.meta_header.meta_data_id = format::MakeMetaDataId(
                        format::ApiFamilyId::ApiFamily_D3D12, format::MetaDataType::kSetSwapchainImageStateCommand);
                    header.thread_id    = thread_id_;
                    header.swapchain_id = id;

                    // The object used to create a swap chain is a command queue for DX12. Store the command_queue's
                    // HandleId in the header's device_id field.
                    header.device_id =
                        reinterpret_cast<const IDXGISwapChainInfo*>(saved_swapchain_state->object_info.get())
                            ->command_queue_id;

                    // last_presented_image is used to store the current swapchain buffer index for DX12.
                    header.last_presented_image = saved_swapchain_state->last_presented_image;

                    // last_presented_image is the only need for replay, so nothing is written into
                    // format::SwapchainImageStateInfo.
                    header.image_info_count = 0;

                    output_stream_->Write(&header, sizeof(header));
                }
            }
        });
}

template <>
bool Dx12LoopStateWriter::ProcessObjectReset(Dx12ObjectResetInfo<ID3D12Heap_Wrapper, Dx12SavedObjectState>& reset_info)
{
    GFXRECON_LOG_INFO_ONCE("Processing Heap Reset");

    bool object_exists_before = (reset_info.saved_state != nullptr);
    bool object_exists_after  = (reset_info.wrapper != nullptr);
    bool object_changed       = object_exists_before != object_exists_after;

    if (object_changed)
    {
        // Release the object if it exists after the loop.
        if (object_exists_after)
        {
            WriteAddRefAndReleaseCommands(reset_info.id, reset_info.wrapper->GetRefCount(), 0);
        }
        // Create the object from the saved state if it existed before the loop.
        else if (object_exists_before)
        {
            auto wrapper_info = reinterpret_cast<const ID3D12HeapInfo*>(reset_info.saved_state->object_info.get());
            if (wrapper_info->open_existing_address != nullptr)
            {
                if (!WriteCreateHeapAllocationCmd(wrapper_info->open_existing_address))
                {
                    GFXRECON_LOG_ERROR("Failed to retrieve memory information for address specified to "
                                       "ID3D12Device3::OpenExistingHeapFromAddress (error = %d)",
                                       GetLastError());
                }
            }

            StandardCreateWrite(
                reset_info.id, reset_info.saved_state->object_info.get(), reset_info.saved_state->ref_count);
        }
    }

    return object_changed;
}

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)
