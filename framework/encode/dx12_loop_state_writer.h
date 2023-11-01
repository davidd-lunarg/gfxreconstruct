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
    // Process existing objects at the end of the loop. Return whether or not the current object state differs from the
    // saved object state.
    template <typename W, typename S>
    bool ProcessObjectReset(Dx12ObjectResetInfo<W, S>& reset_info)
    {
        bool object_exists_before = (reset_info.saved_state != nullptr);
        bool object_exists_after  = (reset_info.wrapper != nullptr);
        bool object_changed       = object_exists_before != object_exists_after;

        // Default behavior to reset an object is to release it then recreate it. Specializations of WriteObjectReset
        // may take a more nuanced approach.
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
                StandardCreateWrite(
                    reset_info.id, reset_info.saved_state->object_info.get(), reset_info.saved_state->ref_count);
            }
        }

        return object_changed;
    }

    template <typename Wrapper, typename SavedState>
    void ProcessObjectsReset(const Dx12StateTable& state_table)
    {
        saved_state_->VisitObjectsForReset<Wrapper, SavedState>(
            state_table, [&](Dx12ObjectResetInfo<Wrapper, SavedState> reset_info) {
                auto id                 = reset_info.id;
                auto object_wrapper     = reset_info.wrapper;
                auto saved_object_state = reset_info.saved_state;

                if (!ProcessObjectReset(reset_info))
                {
                    // The object exists before and after the loop and didn't change. Fixup object ref counts.
                    if (reset_info.saved_state != nullptr)
                    {
                        GFXRECON_ASSERT(object_wrapper->GetCaptureId() == id);
                        WriteAddRefAndReleaseCommands(id, object_wrapper->GetRefCount(), saved_object_state->ref_count);
                    }
                }
            });
    }

    template <typename Wrapper>
    void StandardProcessObjectsReset(const Dx12StateTable& state_table)
    {
        ProcessObjectsReset<Wrapper, Dx12SavedObjectState>(state_table);
    }

    // Fence processing
    template <>
    bool ProcessObjectReset(Dx12ObjectResetInfo<ID3D12Fence_Wrapper, Dx12SavedFenceState>& reset_info);

    // Command list processing
    void WriteCommandListState(const Dx12StateTable& state_table);

    // Swap chain processing
    void WriteSwapChainState(const Dx12StateTable& state_table);

    // Heap processing
    template <>
    bool ProcessObjectReset(Dx12ObjectResetInfo<ID3D12Heap_Wrapper, Dx12SavedObjectState>& reset_info);

    // Dx12SavedState stores saved object state and state table.
    Dx12SavedState* saved_state_;
};

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_ENCODE_DX12_LOOP_STATE_WRITER_H
