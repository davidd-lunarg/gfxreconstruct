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
    StandardCreateWrite<ID3D12Debug_Wrapper>(state_table);
    StandardCreateWrite<ID3D12Debug1_Wrapper>(state_table);
    // LOOP_TODO: WriteEnableDRED();
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
    // LOOP_TODO: WriteAgsInitialize(ags_state_table);
    // LOOP_TODO: WriteAgsDriverExtensionsDX12CreateDevice(ags_state_table);
#endif // GFXRECON_AGS_SUPPORT

    // Device
    StandardCreateWrite<ID3D12Device_Wrapper>(state_table);

    // Agility SDK constructs
    StandardCreateWrite<ID3D12DeviceFactory_Wrapper>(state_table);
    StandardCreateWrite<ID3D12DeviceConfiguration_Wrapper>(state_table);

    // Write this out before rendering begins
    // This ensures the replayer gets a chance to process the metadata command
    // LOOP_TODO: D3D12CaptureManager::Get()->WriteDxgiAdapterInfo();
    // LOOP_TODO: D3D12CaptureManager::Get()->WriteDx12DriverInfo();
    // LOOP_TODO: D3D12CaptureManager::Get()->WriteDx12RuntimeInfo();

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
    // LOOP_TODO: WriteHeapState(state_table);

    // Root signatures
    StandardCreateWrite<ID3D12RootSignature_Wrapper>(state_table);
    StandardCreateWrite<ID3D12RootSignatureDeserializer_Wrapper>(state_table);
    StandardCreateWrite<ID3D12VersionedRootSignatureDeserializer_Wrapper>(state_table);

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
    // LOOP_TODO: WriteAccelerationStructuresState(state_table);

    // Command lists
    StandardCreateWrite<ID3D12CommandAllocator_Wrapper>(state_table);
    StandardCreateWrite<ID3D12CommandSignature_Wrapper>(state_table);
    // LOOP_TODO: WriteResidencyPriority(state_table);
    WriteCommandListState(state_table);

    // TODO: Determine dependencies for creation of ID3D12VirtualizationGuestDevice
    // Since the dependency chain is unclear, just write their state at the very end
    StandardCreateWrite<ID3D12VirtualizationGuestDevice_Wrapper>(state_table);

    marker.marker_type = format::kEndMarker;
    output_stream_->Write(&marker, sizeof(marker));

    saved_state_ = nullptr;
}

void Dx12LoopStateWriter::WriteFenceState(const Dx12StateTable& state_table)
{
    saved_state_->VisitWrappersForReset<ID3D12Fence_Wrapper>(
        state_table, [&](format::HandleId id, ID3D12Fence_Wrapper* fence_wrapper) {
            auto saved_fence_state = saved_state_->GetSavedFenceState(id);
            if (saved_fence_state == nullptr)
            {
                GFXRECON_ASSERT(fence_wrapper != nullptr);

                // If the object doesn't exist in the saved state, release it.
                WriteAddRefAndReleaseCommands(id, fence_wrapper->GetRefCount(), 0);
            }
            else
            {
                // If the object only exists in the saved state, recreate it.
                if (fence_wrapper == nullptr)
                {
                    Dx12StateWriterBase::StandardCreateWrite(
                        id, saved_fence_state->object_info.get(), saved_fence_state->ref_count);
                }
                // If the object exists in both the saved and current state, match ref count to saved state.
                else
                {
                    GFXRECON_ASSERT(fence_wrapper->GetCaptureId() == id);

                    WriteAddRefAndReleaseCommands(id, fence_wrapper->GetRefCount(), saved_fence_state->ref_count);

                    auto fence      = fence_wrapper->GetWrappedObjectAs<ID3D12Fence>();
                    auto fence_info = fence_wrapper->GetObjectInfo();

                    UINT64 current_completed_fence_value = fence->GetCompletedValue();

                    UINT64 max_pending_event_signal_value = 0;
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
                        WriteMethodCall(format::ApiCallId::ApiCall_ID3D12Fence_Signal,
                                        fence_wrapper->GetCaptureId(),
                                        &parameter_stream_);
                        parameter_stream_.Reset();
                    }
                }

                // Write call to signal the fence to its saved value.
                encoder_.EncodeUInt64Value(saved_fence_state->completed_value);
                encoder_.EncodeInt32Value(S_OK);
                WriteMethodCall(
                    format::ApiCallId::ApiCall_ID3D12Fence_Signal, fence_wrapper->GetCaptureId(), &parameter_stream_);
                parameter_stream_.Reset();
            }
        });
}

void Dx12LoopStateWriter::WriteCommandListState(const Dx12StateTable& state_table)
{
    std::vector<CreateInfo<ID3D12CommandListInfo>> bundle_command_lists;
    std::vector<CreateInfo<ID3D12CommandListInfo>> direct_command_lists;

    saved_state_->VisitWrappersForReset<ID3D12CommandList_Wrapper>(
        state_table, [&](format::HandleId id, ID3D12CommandList_Wrapper* list_wrapper) {
            GFXRECON_ASSERT(list_wrapper != nullptr);
            GFXRECON_ASSERT(list_wrapper->GetWrappedObject() != nullptr);
            GFXRECON_ASSERT(list_wrapper->GetObjectInfo() != nullptr);

            // Release all command lists.
            if (list_wrapper != nullptr)
            {
                WriteAddRefAndReleaseCommands(id, list_wrapper->GetRefCount(), 0);
            }

            auto saved_list_state = saved_state_->GetSavedCommandListState(id);
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
    saved_state_->VisitWrappersForReset<IDXGISwapChain_Wrapper>(
        state_table, [&](format::HandleId id, IDXGISwapChain_Wrapper* swapchain_wrapper) {
            auto saved_swapchain_state = saved_state_->GetSavedSwapChainState(id);
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

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)
