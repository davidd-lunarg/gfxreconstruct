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

#include "encode/dx12_state_writer_base.h"

#include "generated/generated_dx12_api_call_encoders.h"
#include "encode/custom_dx12_struct_encoders.h"
#include "encode/custom_dx12_struct_unwrappers.h"
#include "encode/struct_pointer_encoder.h"
#include "encode/d3d12_capture_manager.h"
#include "graphics/dx12_resource_data_util.h"
#include "graphics/dx12_util.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

Dx12StateWriterBase::Dx12StateWriterBase(util::FileOutputStream* output_stream,
                                         util::Compressor*       compressor,
                                         format::ThreadId        thread_id) :
    output_stream_(output_stream),
    compressor_(compressor), thread_id_(thread_id), encoder_(&parameter_stream_)
{
    assert(output_stream != nullptr);
}

Dx12StateWriterBase::~Dx12StateWriterBase() {}

void Dx12StateWriterBase::StandardCreateWrite(format::HandleId     object_id,
                                              const DxWrapperInfo* wrapper_info,
                                              unsigned long        target_ref_count)
{
    if (wrapper_info->create_object_id == format::kNullHandleId)
    {
        WriteFunctionCall(wrapper_info->create_call_id, wrapper_info->create_parameters.get());
    }
    else
    {
        bool create_temp_object_dependency = ((wrapper_info->create_object_info != nullptr) &&
                                              (wrapper_info->create_object_info->GetWrapper() == nullptr));

        // Write a create call for the parent object if its wrapper has been destroyed.
        if (create_temp_object_dependency)
        {
            StandardCreateWrite(wrapper_info->create_object_id, wrapper_info->create_object_info.get(), 1);
        }

        WriteMethodCall(
            wrapper_info->create_call_id, wrapper_info->create_object_id, wrapper_info->create_parameters.get());

        // Release any temporarily created parent object.
        if (create_temp_object_dependency)
        {
            WriteReleaseCommand(wrapper_info->create_object_id, 0);
        }
    }

    WriteAddRefAndReleaseCommands(object_id, 1, target_ref_count);
    WritePrivateData(object_id, *wrapper_info);
}

void Dx12StateWriterBase::WriteFunctionCall(format::ApiCallId call_id, util::MemoryOutputStream* parameter_buffer)
{
    assert(parameter_buffer != nullptr);

    bool                                 not_compressed      = true;
    format::CompressedFunctionCallHeader compressed_header   = {};
    format::FunctionCallHeader           uncompressed_header = {};
    size_t                               uncompressed_size   = parameter_buffer->GetDataSize();
    size_t                               header_size         = 0;
    const void*                          header_pointer      = nullptr;
    size_t                               data_size           = 0;
    const void*                          data_pointer        = nullptr;

    if (compressor_ != nullptr)
    {
        size_t packet_size = 0;
        size_t compressed_size =
            compressor_->Compress(uncompressed_size, parameter_buffer->GetData(), &compressed_parameter_buffer_, 0);

        if ((0 < compressed_size) && (compressed_size < uncompressed_size))
        {
            data_pointer   = reinterpret_cast<const void*>(compressed_parameter_buffer_.data());
            data_size      = compressed_size;
            header_pointer = reinterpret_cast<const void*>(&compressed_header);
            header_size    = sizeof(format::CompressedFunctionCallHeader);

            compressed_header.block_header.type = format::BlockType::kCompressedFunctionCallBlock;
            compressed_header.api_call_id       = call_id;
            compressed_header.thread_id         = thread_id_;
            compressed_header.uncompressed_size = uncompressed_size;

            packet_size += sizeof(compressed_header.api_call_id) + sizeof(compressed_header.uncompressed_size) +
                           sizeof(compressed_header.thread_id) + compressed_size;

            compressed_header.block_header.size = packet_size;
            not_compressed                      = false;
        }
    }

    if (not_compressed)
    {
        size_t packet_size = 0;
        data_pointer       = reinterpret_cast<const void*>(parameter_buffer->GetData());
        data_size          = uncompressed_size;
        header_pointer     = reinterpret_cast<const void*>(&uncompressed_header);
        header_size        = sizeof(format::FunctionCallHeader);

        uncompressed_header.block_header.type = format::BlockType::kFunctionCallBlock;
        uncompressed_header.api_call_id       = call_id;
        uncompressed_header.thread_id         = thread_id_;

        packet_size += sizeof(uncompressed_header.api_call_id) + sizeof(uncompressed_header.thread_id) + data_size;

        uncompressed_header.block_header.size = packet_size;
    }

    // Write appropriate function call block header.
    output_stream_->Write(header_pointer, header_size);

    // Write parameter data.
    output_stream_->Write(data_pointer, data_size);
}

void Dx12StateWriterBase::WriteMethodCall(format::ApiCallId         call_id,
                                          format::HandleId          call_object_id,
                                          util::MemoryOutputStream* parameter_buffer)
{
    assert(parameter_buffer != nullptr);

    bool                               not_compressed      = true;
    format::CompressedMethodCallHeader compressed_header   = {};
    format::MethodCallHeader           uncompressed_header = {};
    size_t                             uncompressed_size   = parameter_buffer->GetDataSize();
    size_t                             header_size         = 0;
    const void*                        header_pointer      = nullptr;
    size_t                             data_size           = 0;
    const void*                        data_pointer        = nullptr;

    if (compressor_ != nullptr)
    {
        size_t packet_size = 0;
        size_t compressed_size =
            compressor_->Compress(uncompressed_size, parameter_buffer->GetData(), &compressed_parameter_buffer_, 0);

        if ((compressed_size > 0) && (compressed_size < uncompressed_size))
        {
            data_pointer   = reinterpret_cast<const void*>(compressed_parameter_buffer_.data());
            data_size      = compressed_size;
            header_pointer = reinterpret_cast<const void*>(&compressed_header);
            header_size    = sizeof(format::CompressedMethodCallHeader);

            compressed_header.block_header.type = format::BlockType::kCompressedMethodCallBlock;
            compressed_header.api_call_id       = call_id;
            compressed_header.object_id         = call_object_id;
            compressed_header.thread_id         = thread_id_;
            compressed_header.uncompressed_size = uncompressed_size;

            packet_size += sizeof(compressed_header.api_call_id) + sizeof(compressed_header.object_id) +
                           sizeof(compressed_header.uncompressed_size) + sizeof(compressed_header.thread_id) +
                           compressed_size;

            compressed_header.block_header.size = packet_size;
            not_compressed                      = false;
        }
    }

    if (not_compressed)
    {
        size_t packet_size = 0;
        data_pointer       = reinterpret_cast<const void*>(parameter_buffer->GetData());
        data_size          = uncompressed_size;
        header_pointer     = reinterpret_cast<const void*>(&uncompressed_header);
        header_size        = sizeof(format::MethodCallHeader);

        uncompressed_header.block_header.type = format::BlockType::kMethodCallBlock;
        uncompressed_header.api_call_id       = call_id;
        uncompressed_header.object_id         = call_object_id;
        uncompressed_header.thread_id         = thread_id_;

        packet_size += sizeof(uncompressed_header.api_call_id) + sizeof(compressed_header.object_id) +
                       sizeof(uncompressed_header.thread_id) + data_size;

        uncompressed_header.block_header.size = packet_size;
    }

    // Write appropriate function call block header.
    output_stream_->Write(header_pointer, header_size);

    // Write parameter data.
    output_stream_->Write(data_pointer, data_size);
}

void Dx12StateWriterBase::WriteAddRefAndReleaseCommands(format::HandleId id,
                                                        unsigned long    current_ref_count,
                                                        unsigned long    target_ref_count)
{
    // Add AddRef/Release commands as needed to set object ref count for replay.
    for (unsigned long i = current_ref_count; i < target_ref_count; ++i)
    {
        WriteAddRefCommand(id, i + 1);
    }
    for (unsigned long i = current_ref_count; i > target_ref_count; --i)
    {
        WriteReleaseCommand(id, i - 1);
    }
}

void Dx12StateWriterBase::WritePrivateData(format::HandleId handle_id, const DxWrapperInfo& wrapper_info)
{
    for (auto& data : wrapper_info.private_datas)
    {
        uint32_t data_size = static_cast<uint32_t>(data.second.size());
        EncodeStruct(&encoder_, data.first);
        encoder_.EncodeUInt32Value(data_size);
        encoder_.EncodeVoidArray(data.second.data(), data_size);
        encoder_.EncodeInt32Value(S_OK);
        if (wrapper_info.IsDxgi())
        {
            WriteMethodCall(format::ApiCallId::ApiCall_IDXGIObject_SetPrivateData, handle_id, &parameter_stream_);
        }
        else
        {
            WriteMethodCall(format::ApiCallId::ApiCall_ID3D12Object_SetPrivateData, handle_id, &parameter_stream_);
        }
        parameter_stream_.Reset();
    }
}

void Dx12StateWriterBase::WriteAddRefCommand(format::HandleId handle_id, unsigned long result_ref_count)
{
    encoder_.EncodeUInt32Value(result_ref_count);
    WriteMethodCall(format::ApiCallId::ApiCall_IUnknown_AddRef, handle_id, &parameter_stream_);
    parameter_stream_.Reset();
}

void Dx12StateWriterBase::WriteReleaseCommand(format::HandleId handle_id, unsigned long result_ref_count)
{
    encoder_.EncodeUInt32Value(result_ref_count);
    WriteMethodCall(format::ApiCallId::ApiCall_IUnknown_Release, handle_id, &parameter_stream_);
    parameter_stream_.Reset();
}

void Dx12StateWriterBase::WaitForCommandQueues(const Dx12StateTable& state_table)
{
    const UINT64 kSignalValue = 1;

    state_table.VisitWrappers([&](format::HandleId id, ID3D12CommandQueue_Wrapper* queue_wrapper) {
        assert(queue_wrapper != nullptr);
        assert(queue_wrapper->GetWrappedObject() != nullptr);

        auto queue  = queue_wrapper->GetWrappedObjectAs<ID3D12CommandQueue>();
        auto result = graphics::dx12::WaitForQueue(queue);
        if (!SUCCEEDED(result))
        {
            GFXRECON_LOG_ERROR("Failed to wait for ID3D12CommandQueue (id = %" PRIu64
                               ") to complete pending work for trim state writing. (error = %x)",
                               queue_wrapper->GetCaptureId(),
                               result);
        }
    });
}

void Dx12StateWriterBase::WriteCommandListState(
    const std::vector<CreateInfo<ID3D12CommandListInfo>>& bundle_command_lists,
    const std::vector<CreateInfo<ID3D12CommandListInfo>>& direct_command_lists,
    const Dx12StateTable&                                 state_table)
{
    std::vector<CreateInfo<ID3D12CommandListInfo>> open_command_lists;

    // Write closed bundle command lists. Save open command lists to be written afterward.
    for (auto create_info : bundle_command_lists)
    {
        if (create_info.object_info->is_closed)
        {
            WriteCommandListCreation(create_info);
            WriteCommandListCommands(create_info.object_id, create_info.object_info, state_table);
        }
        else
        {
            open_command_lists.push_back(create_info);
        }
    }

    // Write closed primary/direct command lists. Save open command lists to be written afterward.
    for (auto create_info : direct_command_lists)
    {
        if (create_info.object_info->is_closed)
        {
            WriteCommandListCreation(create_info);
            WriteCommandListCommands(create_info.object_id, create_info.object_info, state_table);
        }
        else
        {
            open_command_lists.push_back(create_info);
        }
    }

    // Write open command lists.
    for (auto create_info : open_command_lists)
    {
        // If the command list is open and has been reset since creation, create and close it here.
        const auto* list_info = create_info.object_info;
        if (list_info->was_reset)
        {
            WriteCommandListCreation(create_info);
        }
    }
    for (auto create_info : open_command_lists)
    {
        // Write creation calls for command lists that were never reset.
        const auto* list_info = create_info.object_info;
        if (!list_info->was_reset)
        {
            WriteCommandListCreation(create_info);
        }

        // Write commands for all open command lists.
        WriteCommandListCommands(create_info.object_id, list_info, state_table);
    }
}

void Dx12StateWriterBase::WriteCommandListCreation(const CreateInfo<ID3D12CommandListInfo>& create_info)
{
    // Write call to create the command list.
    StandardCreateWrite(create_info);
    auto list_info = create_info.object_info;

    // If the command list was created open and reset since creation, write a command to close it. This frees up the
    // command allocator used in creation. This list's command_data will contain a reset, possibly with a different
    // command allocator.
    bool created_open = (list_info->create_call_id == format::ApiCall_ID3D12Device_CreateCommandList);
    if (list_info->was_reset && created_open)
    {
        WriteCommandListClose(create_info.object_id);
    }
}

void Dx12StateWriterBase::WriteCommandListCommands(format::HandleId             list_id,
                                                   const ID3D12CommandListInfo* list_info,
                                                   const Dx12StateTable&        state_table)
{
    bool write_commands = CheckCommandListObjects(list_info, state_table);

    // Write each of the commands that was recorded for the command buffer.
    size_t         offset    = 0;
    size_t         data_size = list_info->command_data.GetDataSize();
    const uint8_t* data      = list_info->command_data.GetData();

    while (offset < data_size)
    {
        const size_t*            parameter_size = reinterpret_cast<const size_t*>(&data[offset]);
        const format::ApiCallId* call_id = reinterpret_cast<const format::ApiCallId*>(&data[offset] + sizeof(size_t));
        const uint8_t*           parameter_data = &data[offset] + (sizeof(size_t) + sizeof(format::ApiCallId));

        bool write_current_command = write_commands;

        if ((*call_id) == format::ApiCallId::ApiCall_ID3D12GraphicsCommandList_Reset)
        {
            GFXRECON_ASSERT(list_info->was_reset);

            // command_data is cleared after each reset, so only the first command can be a reset.
            GFXRECON_ASSERT(offset == 0);

            // Always write the reset command.
            write_current_command = true;
        }
        else if ((*call_id) == format::ApiCallId::ApiCall_ID3D12GraphicsCommandList_Close)
        {
            GFXRECON_ASSERT(list_info->is_closed);

            // Always write the close command.
            write_current_command = true;
        }
#ifdef GFXRECON_AGS_SUPPORT
        else if (format::GetApiCallFamily(*call_id) == format::ApiFamilyId::ApiFamily_AGS)
        {
            if (write_current_command)
            {
                // Ags function call, targeting command list.
                parameter_stream_.Write(parameter_data, (*parameter_size));
                WriteFunctionCall((*call_id), &parameter_stream_);
                parameter_stream_.Reset();

                // The output below, in this case, should be skipped.
                write_current_command = false;
            }
        }
#endif // GFXRECON_AGS_SUPPORT

        if (write_current_command)
        {
            parameter_stream_.Write(parameter_data, (*parameter_size));
            WriteMethodCall((*call_id), list_id, &parameter_stream_);
            parameter_stream_.Reset();
        }
        offset += sizeof(size_t) + sizeof(format::ApiCallId) + (*parameter_size);
    }

    GFXRECON_ASSERT(offset == data_size);
}

void Dx12StateWriterBase::WriteCommandListClose(format::HandleId list_id)
{
    encoder_.EncodeUInt32Value(S_OK);
    WriteMethodCall(format::ApiCallId::ApiCall_ID3D12GraphicsCommandList_Close, list_id, &parameter_stream_);
    parameter_stream_.Reset();
}

bool Dx12StateWriterBase::CheckCommandListObjects(const ID3D12CommandListInfo* list_info,
                                                  const Dx12StateTable&        state_table)
{
    // Ignore commands that reference destroyed objects.
    for (uint32_t i = 0; i < D3D12GraphicsCommandObjectType::NumObjectTypes; ++i)
    {
        for (auto id : list_info->command_objects[i])
        {
            if (!CheckCommandListObject(static_cast<D3D12GraphicsCommandObjectType>(i), id, state_table))
            {
                return false;
            }
        }
    }

    for (auto cpu_descriptor_handle : list_info->command_cpu_descriptor_handles)
    {
        auto* descriptor_info = GetDescriptorInfo(cpu_descriptor_handle);
        if (!descriptor_info)
        {
            return false;
        }

        if (!CheckDescriptorObjects(const_cast<DxDescriptorInfo&>(*descriptor_info), state_table))
        {
            return false;
        }
    }

    for (auto gpu_va : list_info->command_gpu_virtual_addresses)
    {
        if (!CheckGpuVa(gpu_va))
        {
            return false;
        }
    }

    return true;
}

bool Dx12StateWriterBase::CheckCommandListObject(D3D12GraphicsCommandObjectType object_type,
                                                 format::HandleId               handle_id,
                                                 const Dx12StateTable&          state_table)
{
    switch (object_type)
    {
        case D3D12GraphicsCommandObjectType::ID3D12CommandAllocatorObject:
            return (state_table.GetID3D12CommandAllocator_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12PipelineStateObject:
            return (state_table.GetID3D12PipelineState_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12ResourceObject:
            return (state_table.GetID3D12Resource_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12GraphicsCommandListObject:
            return (state_table.GetID3D12CommandList_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12DescriptorHeapObject:
            return (state_table.GetID3D12DescriptorHeap_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12RootSignatureObject:
            return (state_table.GetID3D12RootSignature_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12QueryHeapObject:
            return (state_table.GetID3D12QueryHeap_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12ProtectedResourceSessionObject:
            return (state_table.GetID3D12ProtectedResourceSession_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12MetaCommandObject:
            return (state_table.GetID3D12MetaCommand_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12StateObjectObject:
            return (state_table.GetID3D12StateObject_Wrapper(handle_id) != nullptr);
        case D3D12GraphicsCommandObjectType::ID3D12CommandSignatureObject:
            return (state_table.GetID3D12CommandSignature_Wrapper(handle_id) != nullptr);
        default:
            GFXRECON_LOG_ERROR("State write is skipping unrecognized object type when checking handles "
                               "referenced by command buffers");
            GFXRECON_ASSERT(false);
            return false;
    }
}

bool Dx12StateWriterBase::CheckGpuVa(D3D12_GPU_VIRTUAL_ADDRESS address)
{
    if (address != 0)
    {
        bool found = false;
        gpu_va_map_.Map(address, nullptr, &found);
        return found;
    }
    return true;
}

bool Dx12StateWriterBase::CheckDescriptorObjects(const DxDescriptorInfo& descriptor_info,
                                                 const Dx12StateTable&   state_table)
{
    // Ignore descriptors that reference destroyed resource object.
    for (auto descriptor_resource_id : descriptor_info.resource_ids)
    {
        if ((descriptor_resource_id != format::kNullHandleId) &&
            !CheckCommandListObject(ID3D12ResourceObject, descriptor_resource_id, state_table))
        {
            return false;
        }
    }

    // Ignore descriptors that reference GPU VAs that are no longer valid.
    if (!CheckGpuVa(descriptor_info.resource_gpu_va))
    {
        return false;
    }

    return true;
}

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)
