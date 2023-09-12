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

#ifndef GFXRECON_ENCODE_DX12_LOOP_STATE_WRITER_H
#define GFXRECON_ENCODE_DX12_LOOP_STATE_WRITER_H

#include "encode/dx12_state_writer_base.h"

#ifdef GFXRECON_AGS_SUPPORT
#include "encode/custom_ags_state_table.h"
#endif // GFXRECON_AGS_SUPPORT
#include "encode/parameter_encoder.h"
#include "encode/dx12_saved_state.h"
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

class Dx12LoopStateWriter : public Dx12StateWriterBase
{
  public:
    Dx12LoopStateWriter(util::FileOutputStream* output_stream,
                        util::Compressor*       compressor,
                        format::ThreadId        thread_id);

    ~Dx12LoopStateWriter();

#ifdef GFXRECON_AGS_SUPPORT
    void WriteState(const Dx12StateTable& state_table,
                    const AgsStateTable&  ags_state_table,
                    uint64_t              frame_number,
                    Dx12SavedState*       saved_state);
#else
    void WriteState(const Dx12StateTable& state_table, uint64_t frame_number, Dx12SavedState* saved_state);
#endif // GFXRECON_AGS_SUPPORT

  protected:
    template <typename Wrapper>
    void StandardCreateWrite(const Dx12StateTable& state_table)
    {
        saved_state_->VisitWrappersForReset<Wrapper>(state_table, [&](format::HandleId id, const Wrapper* wrapper) {
            auto saved_object_state = saved_state_->GetSavedObjectState(id);
            if (saved_object_state == nullptr)
            {
                // If the object doesn't exist in the saved state, release it.
                WriteAddRefAndReleaseCommands(wrapper->GetCaptureId(), wrapper->GetRefCount(), 0);
            }
            else
            {
                // If the object only exists in the saved state, recreate it.
                if (wrapper == nullptr)
                {
                    GFXRECON_ASSERT(saved_object_state != nullptr);
                    Dx12StateWriterBase::StandardCreateWrite(
                        id, *saved_object_state->object_info.get(), saved_object_state->ref_count);
                }
                // If the object exists in both the saved and current state, match ref count to saved state.
                else
                {
                    GFXRECON_ASSERT(wrapper->GetCaptureId() == id);

                    WriteAddRefAndReleaseCommands(id, wrapper->GetRefCount(), saved_object_state->ref_count);
                }
            }
        });
    }

    void WriteFenceState(const Dx12StateTable& state_table);

    void WriteCommandListState(const Dx12StateTable& state_table);

    void WriteCommandListCreation(format::HandleId id, const ID3D12CommandList_Wrapper* list_wrapper);

    void WriteSwapChainState(const Dx12StateTable& state_table);

    Dx12SavedState* saved_state_;
};

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_ENCODE_DX12_LOOP_STATE_WRITER_H
