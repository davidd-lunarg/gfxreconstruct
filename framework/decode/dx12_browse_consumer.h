/*
** Copyright (c) 2023 LunarG, Inc.
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

#ifndef GFXRECON_DECODE_DX12_BROWSE_CONSUMER_H
#define GFXRECON_DECODE_DX12_BROWSE_CONSUMER_H

#include "generated/generated_dx12_decoder.h"
#include "graphics/dx12_dump_resources.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(decode)

struct TrackDumpDrawcall
{
    format::HandleId command_list_id{ format::kNullHandleId };
    uint64_t         begin_code_index{ 0 };
    uint64_t         close_code_index{ 0 };
    uint64_t         begin_renderpass_code_index{ 0 };
    uint64_t         set_render_targets_code_index{ 0 };
    format::HandleId root_signature_handle_id{ format::kNullHandleId };

    // vertex
    std::vector<D3D12_VERTEX_BUFFER_VIEW> captured_vertex_buffer_views;

    // index
    D3D12_INDEX_BUFFER_VIEW captured_index_buffer_view{};

    // descriptor
    std::vector<format::HandleId>               descriptor_heap_ids;
    std::map<UINT, D3D12_GPU_DESCRIPTOR_HANDLE> captured_descriptor_gpu_handles;

    // ExecuteIndirect
    format::HandleId exe_indirect_argument_id{ format::kNullHandleId };
    uint64_t         exe_indirect_argument_offset{ 0 };
    format::HandleId exe_indirect_count_id{ format::kNullHandleId };
    uint64_t         exe_indirect_count_offset{ 0 };

    uint64_t drawcall_code_index{ 0 };
    uint64_t execute_code_index{ 0 };

    void Clear()
    {
        captured_vertex_buffer_views.clear();
        descriptor_heap_ids.clear();
        captured_descriptor_gpu_handles.clear();
    }
};

struct TrackDumpCommandList
{
    uint64_t         begin_code_index{ 0 };
    uint64_t         current_begin_renderpass_code_index{ 0 };
    uint64_t         current_set_render_targets_code_index{ 0 };
    format::HandleId current_root_signature_handle_id{ format::kNullHandleId };

    // vertex
    std::vector<D3D12_VERTEX_BUFFER_VIEW> current_captured_vertex_buffer_views;

    // index
    D3D12_INDEX_BUFFER_VIEW current_captured_index_buffer_view{};

    // descriptor
    std::vector<format::HandleId>               current_descriptor_heap_ids;
    std::map<UINT, D3D12_GPU_DESCRIPTOR_HANDLE> current_captured_descriptor_gpu_handles;

    // render target
    // Track render target info in replay, not here.
    // Because the useful info is replay cpuDescriptor. It's only available in replay.

    std::vector<TrackDumpDrawcall> track_dump_drawcalls;

    void Clear()
    {
        begin_code_index                      = 0;
        current_begin_renderpass_code_index   = 0;
        current_set_render_targets_code_index = 0;
        current_captured_vertex_buffer_views.clear();
        current_captured_index_buffer_view = {};
        current_descriptor_heap_ids.clear();
        current_captured_descriptor_gpu_handles.clear();
        track_dump_drawcalls.clear();
    }
};

class Dx12BrowseConsumer : public Dx12Consumer
{
  public:
    Dx12BrowseConsumer() {}

    void SetDumpTarget(const DumpResourcesTarget& dump_resources_target)
    {
        dump_resources_target_ = dump_resources_target;
    }

    TrackDumpDrawcall* GetTrackDumpTarget()
    {
        if (track_submit_index_ <= dump_resources_target_.submit_index)
        {
            GFXRECON_LOG_FATAL("The target submit index(%d) of dump resources is out of range(%d).",
                               dump_resources_target_.submit_index,
                               track_submit_index_);
            GFXRECON_ASSERT(track_submit_index_ > dump_resources_target_.submit_index);
        }

        auto it = track_commandlist_infos_.find(target_command_list_);
        if (it != track_commandlist_infos_.end())
        {
            auto drawcall_size = it->second.track_dump_drawcalls.size();
            GFXRECON_ASSERT(drawcall_size > dump_resources_target_.drawcall_index);

            return &(it->second.track_dump_drawcalls[dump_resources_target_.drawcall_index]);
        }
        return nullptr;
    }

    virtual void Process_ID3D12Device_CreateCommandList(const ApiCallInfo&           call_info,
                                                        format::HandleId             object_id,
                                                        HRESULT                      return_value,
                                                        UINT                         nodeMask,
                                                        D3D12_COMMAND_LIST_TYPE      type,
                                                        format::HandleId             pCommandAllocator,
                                                        format::HandleId             pInitialState,
                                                        Decoded_GUID                 riid,
                                                        HandlePointerDecoder<void*>* ppCommandList)
    {
        InitializeTracking(call_info, *ppCommandList->GetPointer());
    }

    virtual void Process_ID3D12GraphicsCommandList_Reset(const ApiCallInfo& call_info,
                                                         format::HandleId   object_id,
                                                         HRESULT            return_value,
                                                         format::HandleId   pAllocator,
                                                         format::HandleId   pInitialState)
    {
        InitializeTracking(call_info, object_id);
    }

    virtual void Process_ID3D12GraphicsCommandList4_BeginRenderPass(
        const ApiCallInfo&                                                  call_info,
        format::HandleId                                                    object_id,
        UINT                                                                NumRenderTargets,
        StructPointerDecoder<Decoded_D3D12_RENDER_PASS_RENDER_TARGET_DESC>* pRenderTargets,
        StructPointerDecoder<Decoded_D3D12_RENDER_PASS_DEPTH_STENCIL_DESC>* pDepthStencil,
        D3D12_RENDER_PASS_FLAGS                                             Flags)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_begin_renderpass_code_index   = call_info.index;
                it->second.current_set_render_targets_code_index = 0;
            }
        }
    }

    virtual void Process_ID3D12GraphicsCommandList_OMSetRenderTargets(
        const ApiCallInfo&                                         call_info,
        format::HandleId                                           object_id,
        UINT                                                       NumRenderTargetDescriptors,
        StructPointerDecoder<Decoded_D3D12_CPU_DESCRIPTOR_HANDLE>* pRenderTargetDescriptors,
        BOOL                                                       RTsSingleHandleToDescriptorRange,
        StructPointerDecoder<Decoded_D3D12_CPU_DESCRIPTOR_HANDLE>* pDepthStencilDescriptor)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_set_render_targets_code_index = call_info.index;
                it->second.current_begin_renderpass_code_index   = 0;
            }
        }
    }

    virtual void Process_ID3D12GraphicsCommandList_SetComputeRootSignature(const ApiCallInfo& call_info,
                                                                           format::HandleId   object_id,
                                                                           format::HandleId   pRootSignature)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_root_signature_handle_id = pRootSignature;
            }
        }
    }

    virtual void Process_ID3D12GraphicsCommandList_SetGraphicsRootSignature(const ApiCallInfo& call_info,
                                                                            format::HandleId   object_id,
                                                                            format::HandleId   pRootSignature)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_root_signature_handle_id = pRootSignature;
            }
        }
    }

    virtual void
    Process_ID3D12GraphicsCommandList_IASetVertexBuffers(const ApiCallInfo& call_info,
                                                         format::HandleId   object_id,
                                                         UINT               StartSlot,
                                                         UINT               NumViews,
                                                         StructPointerDecoder<Decoded_D3D12_VERTEX_BUFFER_VIEW>* pViews)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_captured_vertex_buffer_views.resize(NumViews);
                auto views = pViews->GetMetaStructPointer();
                for (uint32_t i = 0; i < NumViews; ++i)
                {
                    it->second.current_captured_vertex_buffer_views[i] = *(views[i].decoded_value);
                }
            }
        }
    }

    virtual void
    Process_ID3D12GraphicsCommandList_IASetIndexBuffer(const ApiCallInfo&                                     call_info,
                                                       format::HandleId                                       object_id,
                                                       StructPointerDecoder<Decoded_D3D12_INDEX_BUFFER_VIEW>* pView)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                auto view                                     = pView->GetMetaStructPointer();
                it->second.current_captured_index_buffer_view = *(view->decoded_value);
            }
        }
    }

    virtual void
    Process_ID3D12GraphicsCommandList_SetDescriptorHeaps(const ApiCallInfo& call_info,
                                                         format::HandleId   object_id,
                                                         UINT               NumDescriptorHeaps,
                                                         HandlePointerDecoder<ID3D12DescriptorHeap*>* ppDescriptorHeaps)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_descriptor_heap_ids.resize(NumDescriptorHeaps);
                auto heap_ids = ppDescriptorHeaps->GetPointer();
                for (uint32_t i = 0; i < NumDescriptorHeaps; ++i)
                {
                    it->second.current_descriptor_heap_ids[i] = heap_ids[i];
                }
                it->second.current_captured_descriptor_gpu_handles.clear();
            }
        }
    }

    virtual void
    Process_ID3D12GraphicsCommandList_SetComputeRootDescriptorTable(const ApiCallInfo& call_info,
                                                                    format::HandleId   object_id,
                                                                    UINT               RootParameterIndex,
                                                                    Decoded_D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_captured_descriptor_gpu_handles[RootParameterIndex] =
                    (*BaseDescriptor.decoded_value);
            }
        }
    }

    virtual void
    Process_ID3D12GraphicsCommandList_SetGraphicsRootDescriptorTable(const ApiCallInfo& call_info,
                                                                     format::HandleId   object_id,
                                                                     UINT               RootParameterIndex,
                                                                     Decoded_D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.current_captured_descriptor_gpu_handles[RootParameterIndex] =
                    (*BaseDescriptor.decoded_value);
            }
        }
    }

    virtual void Process_ID3D12GraphicsCommandList_DrawInstanced(const ApiCallInfo& call_info,
                                                                 format::HandleId   object_id,
                                                                 UINT               VertexCountPerInstance,
                                                                 UINT               InstanceCount,
                                                                 UINT               StartVertexLocation,
                                                                 UINT               StartInstanceLocation)
    {
        TrackTargetDrawcall(call_info, object_id);
    }

    virtual void Process_ID3D12GraphicsCommandList_DrawIndexedInstanced(const ApiCallInfo& call_info,
                                                                        format::HandleId   object_id,
                                                                        UINT               IndexCountPerInstance,
                                                                        UINT               InstanceCount,
                                                                        UINT               StartIndexLocation,
                                                                        INT                BaseVertexLocation,
                                                                        UINT               StartInstanceLocation)
    {
        TrackTargetDrawcall(call_info, object_id);
    }

    virtual void Process_ID3D12GraphicsCommandList_Dispatch(const ApiCallInfo& call_info,
                                                            format::HandleId   object_id,
                                                            UINT               ThreadGroupCountX,
                                                            UINT               ThreadGroupCountY,
                                                            UINT               ThreadGroupCountZ)
    {
        TrackTargetDrawcall(call_info, object_id);
    }

    virtual void Process_ID3D12GraphicsCommandList_ExecuteIndirect(const ApiCallInfo& call_info,
                                                                   format::HandleId   object_id,
                                                                   format::HandleId   pCommandSignature,
                                                                   UINT               MaxCommandCount,
                                                                   format::HandleId   pArgumentBuffer,
                                                                   UINT64             ArgumentBufferOffset,
                                                                   format::HandleId   pCountBuffer,
                                                                   UINT64             CountBufferOffset)
    {
        TrackTargetDrawcall(
            call_info, object_id, pArgumentBuffer, ArgumentBufferOffset, pCountBuffer, CountBufferOffset);
    }

    virtual void Process_ID3D12GraphicsCommandList_Close(const ApiCallInfo& call_info,
                                                         format::HandleId   object_id,
                                                         HRESULT            return_value)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                for (auto& drawcall : it->second.track_dump_drawcalls)
                {
                    drawcall.close_code_index = call_info.index;
                }
            }
        }
    }

    virtual void
    Process_ID3D12CommandQueue_ExecuteCommandLists(const ApiCallInfo&                        call_info,
                                                   format::HandleId                          object_id,
                                                   UINT                                      NumCommandLists,
                                                   HandlePointerDecoder<ID3D12CommandList*>* ppCommandLists)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            if (track_submit_index_ == dump_resources_target_.submit_index)
            {
                if (NumCommandLists <= dump_resources_target_.command_index)
                {
                    GFXRECON_LOG_FATAL("The target command index(%d) of dump resources is out of range(%d).",
                                       dump_resources_target_.command_index,
                                       NumCommandLists);
                    GFXRECON_ASSERT(NumCommandLists > dump_resources_target_.command_index);
                }
                auto command_lists   = ppCommandLists->GetPointer();
                target_command_list_ = command_lists[dump_resources_target_.command_index];

                auto it = track_commandlist_infos_.find(target_command_list_);
                GFXRECON_ASSERT(it != track_commandlist_infos_.end());

                auto drawcall_size = it->second.track_dump_drawcalls.size();
                if (drawcall_size <= dump_resources_target_.drawcall_index)
                {
                    GFXRECON_LOG_FATAL("The target drawcall index(%d) of dump resources is out of range(%d).",
                                       dump_resources_target_.drawcall_index,
                                       drawcall_size);
                    GFXRECON_ASSERT(drawcall_size > dump_resources_target_.drawcall_index);
                }
                it->second.track_dump_drawcalls[dump_resources_target_.drawcall_index].execute_code_index =
                    call_info.index;
            }
            ++track_submit_index_;
        }
    }

    virtual bool IsComplete(uint64_t block_index) override { return (target_command_list_ != format::kNullHandleId); }

  private:
    DumpResourcesTarget dump_resources_target_{};
    uint32_t            track_submit_index_{ 0 };
    format::HandleId    target_command_list_{ 0 };

    // Key is commandlist_id. We need to know the commandlist of the info because in a commandlist block
    // between reset and close, it might have the other commandlist's commands.
    std::map<format::HandleId, TrackDumpCommandList> track_commandlist_infos_;

    void InitializeTracking(const ApiCallInfo& call_info, format::HandleId object_id)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                it->second.Clear();
                it->second.begin_code_index = call_info.index;
            }
            else
            {
                TrackDumpCommandList info = {};
                info.begin_code_index     = call_info.index;
                track_commandlist_infos_.insert({ object_id, std::move(info) });
            }
        }
    }

    void TrackTargetDrawcall(const ApiCallInfo& call_info,
                             format::HandleId   object_id,
                             format::HandleId   exe_indirect_argument_id     = format::kNullHandleId,
                             uint64_t           exe_indirect_argument_offset = 0,
                             format::HandleId   exe_indirect_count_id        = format::kNullHandleId,
                             uint64_t           exe_indirect_count_offset    = 0)
    {
        if (target_command_list_ == format::kNullHandleId)
        {
            auto it = track_commandlist_infos_.find(object_id);
            if (it != track_commandlist_infos_.end())
            {
                TrackDumpDrawcall track_drawcall               = {};
                track_drawcall.command_list_id                 = object_id;
                track_drawcall.drawcall_code_index             = call_info.index;
                track_drawcall.begin_code_index                = it->second.begin_code_index;
                track_drawcall.begin_renderpass_code_index     = it->second.current_begin_renderpass_code_index;
                track_drawcall.set_render_targets_code_index   = it->second.current_set_render_targets_code_index;
                track_drawcall.root_signature_handle_id        = it->second.current_root_signature_handle_id;
                track_drawcall.captured_vertex_buffer_views    = it->second.current_captured_vertex_buffer_views;
                track_drawcall.captured_index_buffer_view      = it->second.current_captured_index_buffer_view;
                track_drawcall.descriptor_heap_ids             = it->second.current_descriptor_heap_ids;
                track_drawcall.captured_descriptor_gpu_handles = it->second.current_captured_descriptor_gpu_handles;
                track_drawcall.exe_indirect_argument_id        = exe_indirect_argument_id;
                track_drawcall.exe_indirect_argument_offset    = exe_indirect_argument_offset;
                track_drawcall.exe_indirect_count_id           = exe_indirect_count_id;
                track_drawcall.exe_indirect_count_offset       = exe_indirect_count_offset;
                it->second.track_dump_drawcalls.emplace_back(std::move(track_drawcall));
            }
        }
    }
};

GFXRECON_END_NAMESPACE(decode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_DECODE_DX12_BROWSE_CONSUMER_H