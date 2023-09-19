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

#ifndef GFXRECON_ENCODE_DX12_STATE_WRITER_BASE_H
#define GFXRECON_ENCODE_DX12_STATE_WRITER_BASE_H

#include "encode/parameter_encoder.h"
#include "format/format.h"
#include "graphics/dx12_gpu_va_map.h"
#include "graphics/dx12_resource_data_util.h"
#include "util/compressor.h"
#include "util/defines.h"
#include "util/file_output_stream.h"
#include "util/memory_output_stream.h"
#include "generated/generated_dx12_state_table.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

class Dx12StateWriterBase
{
  public:
    Dx12StateWriterBase(util::FileOutputStream* output_stream,
                        util::Compressor*       compressor,
                        format::ThreadId        thread_id);

    ~Dx12StateWriterBase();

  protected:
    template <typename Wrapper>
    void StandardCreateWrite(const Wrapper& wrapper)
    {
        assert(wrapper != nullptr);
        assert(wrapper->GetObjectInfo() != nullptr);
        assert(wrapper->GetObjectInfo()->create_parameters != nullptr);

        StandardCreateWrite(wrapper->GetCaptureId(), *wrapper->GetObjectInfo().get(), wrapper->GetRefCount());
    }

    void StandardCreateWrite(format::HandleId object_id, const DxWrapperInfo& wrapper_info, unsigned long ref_count);

    void WriteFunctionCall(format::ApiCallId call_id, util::MemoryOutputStream* parameter_buffer);

    void
    WriteMethodCall(format::ApiCallId call_id, format::HandleId object_id, util::MemoryOutputStream* parameter_buffer);

    void
    WriteAddRefAndReleaseCommands(format::HandleId id, unsigned long current_ref_count, unsigned long target_ref_count);

    void WritePrivateData(format::HandleId handle_id, const DxWrapperInfo& wrapper_info);

    void WriteAddRefCommand(format::HandleId handle_id, unsigned long result_ref_count);

    void WriteReleaseCommand(format::HandleId handle_id, unsigned long result_ref_count);

    // Sync to ensure all pending command queues are completed before processing state writing.
    void WaitForCommandQueues(const Dx12StateTable& state_table);

    void WriteCommandListCommands(format::HandleId             list_id,
                                  const ID3D12CommandListInfo* list_info,
                                  const Dx12StateTable&        state_table);

    void WriteCommandListClose(format::HandleId list_id);

    bool CheckCommandListObjects(const ID3D12CommandListInfo* list_info, const Dx12StateTable& state_table);

    bool CheckCommandListObject(D3D12GraphicsCommandObjectType object_type,
                                format::HandleId               handle_id,
                                const Dx12StateTable&          state_table);

    bool CheckGpuVa(D3D12_GPU_VIRTUAL_ADDRESS address);

    bool CheckDescriptorObjects(const DxDescriptorInfo& descriptor_info, const Dx12StateTable& state_table);

    util::FileOutputStream*  output_stream_;
    util::Compressor*        compressor_;
    std::vector<uint8_t>     compressed_parameter_buffer_;
    format::ThreadId         thread_id_;
    util::MemoryOutputStream parameter_stream_;
    ParameterEncoder         encoder_;
    graphics::Dx12GpuVaMap   gpu_va_map_;
};

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_ENCODE_DX12_LOOP_STATE_WRITER_H
