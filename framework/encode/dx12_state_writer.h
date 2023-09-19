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

#ifndef GFXRECON_ENCODE_DX12_STATE_WRITER_H
#define GFXRECON_ENCODE_DX12_STATE_WRITER_H

#include "encode/dx12_state_writer_base.h"

#ifdef GFXRECON_AGS_SUPPORT
#include "encode/custom_ags_state_table.h"
#endif // GFXRECON_AGS_SUPPORT
#include "encode/parameter_encoder.h"
#include "format/format.h"
#include "graphics/dx12_gpu_va_map.h"
#include "graphics/dx12_resource_data_util.h"
#include "util/compressor.h"
#include "util/defines.h"
#include "util/file_output_stream.h"
#include "util/memory_output_stream.h"
#include "generated/generated_dx12_state_table.h"

// TODO: Is the debug code enabled by this define still useful?
#define GFXRECON_DEBUG_WRITTEN_OBJECTS 1

#if GFXRECON_DEBUG_WRITTEN_OBJECTS
#include <unordered_set>
#endif

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

class Dx12StateWriter : public Dx12StateWriterBase
{
  public:
    Dx12StateWriter(util::FileOutputStream* output_stream, util::Compressor* compressor, format::ThreadId thread_id);

    ~Dx12StateWriter();

#ifdef GFXRECON_AGS_SUPPORT
    void WriteState(const Dx12StateTable& state_table, const AgsStateTable& ags_state_table, uint64_t frame_number);
#else
    void WriteState(const Dx12StateTable& state_table, uint64_t frame_number);
#endif // GFXRECON_AGS_SUPPORT


  private:
    struct ResourceSnapshotInfo
    {
        ID3D12Resource_Wrapper* resource_wrapper{ nullptr };
    };

    // TODO: This is similar to the method in VulkanStateWriter. Possibly refactor to share common code.
    template <typename Wrapper>
    void StandardCreateWrite(const Dx12StateTable& state_table)
    {
        std::set<util::MemoryOutputStream*> processed;
        state_table.VisitWrappers([&](format::HandleId id, const Wrapper* wrapper) {
            assert(wrapper != nullptr);
            assert(wrapper->GetObjectInfo() != nullptr);
            assert(wrapper->GetObjectInfo()->create_parameters != nullptr);

            // Filter duplicate entries for calls that create multiple objects, where objects created by the same call
            // all reference the same parameter buffer.
            auto wrapper_info = wrapper->GetObjectInfo();
            if (processed.find(wrapper_info->create_parameters.get()) == processed.end())
            {
                Dx12StateWriterBase::StandardCreateWrite(wrapper);
                processed.insert(wrapper_info->create_parameters.get());
            }
        });
    }

    void WriteHeapState(const Dx12StateTable& state_table);

    // Returns true if memory information was successfully retrieved and written and false otherwise.
    bool WriteCreateHeapAllocationCmd(const void* address);

    void WriteDescriptorState(const Dx12StateTable& state_table);

    void WriteResidencyPriority(const Dx12StateTable& state_table);

    void WriteResourceCreationState(
        const Dx12StateTable&                                                    state_table,
        std::unordered_map<format::HandleId, std::vector<ResourceSnapshotInfo>>& resource_snapshots,
        std::unordered_map<format::HandleId, uint64_t>&                          max_resource_sizes);

    void WriteTileMappings(const Dx12StateTable& state_table, ID3D12ResourceInfo* resource_info);

    void
    WriteResourceSnapshots(const std::unordered_map<format::HandleId, std::vector<ResourceSnapshotInfo>>& snapshots,
                           const std::unordered_map<format::HandleId, uint64_t>& max_resource_sizes);

    void WriteResourceSnapshot(graphics::Dx12ResourceDataUtil* resource_data_util,
                               const ResourceSnapshotInfo&     snapshot);

    void WriteFenceState(const Dx12StateTable& state_table);

    void WriteCommandListState(const Dx12StateTable& state_table);

    void WriteCommandListCreation(const ID3D12CommandList_Wrapper* list_wrapper);

    void WriteSwapChainState(const Dx12StateTable& state_table);

    void WriteEnableDebugLayer();

    void WriteEnableDRED();

    void WriteAccelerationStructuresState(const Dx12StateTable& state_table);

    void WriteAccelerationStructuresState(std::map<uint64_t, const DxAccelerationStructureBuildInfo*> build_infos);

    void WriteStateObjectsState(const Dx12StateTable& state_table);

    void WriteStateObjectPropertiesState(const Dx12StateTable& state_table);

    void WriteStateObjectAndDependency(const format::HandleId                state_object_id,
                                       const ID3D12StateObjectInfo*          state_object_info,
                                       std::unordered_set<format::HandleId>& written_objs,
                                       unsigned long                         ref_count);

#ifdef GFXRECON_AGS_SUPPORT
    void WriteAgsInitialize(const AgsStateTable& ags_state_table);

    void WriteAgsDriverExtensionsDX12CreateDevice(const AgsStateTable& ags_state_table);
#endif // GFXRECON_AGS_SUPPORT

#if GFXRECON_DEBUG_WRITTEN_OBJECTS
    // Track the list of objects that have been written in WriteState.
    std::unordered_set<format::HandleId> written_objects_;
#endif

    // Temporary vectors.
    std::vector<uint8_t>           temp_subresource_data_;
    std::vector<uint64_t>          temp_subresource_sizes_;
    std::vector<uint64_t>          temp_subresource_offsets_;
    std::vector<DxTileMappingInfo> temp_tile_mappings_;
};

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_ENCODE_DX12_STATE_WRITER_H
