/*
** Copyright (c) 2026 LunarG, Inc.
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

// dxr-acceleration-structures: top-level build inputs in the forms a replay must follow to the GPU, on this app's own
// scene (dxr_as.hlsl): a quad per BLAS, at z = 1 (even bands) and z = 1.5 (odd bands), and a tile of four horizontal
// bands, band b an instance of the quad scaled into the band's rows with InstanceID b. Tiles:
//   0 pointers-cpu        ARRAY_OF_POINTERS: the pointer array and the four descs CPU-written into upload buffers (a
//                         buffer per desc), the TLAS rebuilt every frame
//   1 pointers-gpu        the pointer array written by a compute shader from root constants, so at replay it holds
//                         capture addresses unless the replay translates values written on the GPU
//   2 descs-gpu           ARRAY: the descs composed by a compute shader from root constants carrying the BLAS addresses
//   3 pointers-gpu-descs  ARRAY_OF_POINTERS: CPU-written pointers into GPU-composed descs
//   4 pointers-inactive   ARRAY_OF_POINTERS whose bands 1 and 2 point at descs without a BLAS (legal, inactive): their
//                         bands must stay unhit
//   5 update              a TLAS built once with ALLOW_UPDATE and its instances off the tile, refit in place every
//                         frame (PERFORM_UPDATE) from CPU-written descs that move them into their bands, with the
//                         bands' depths swapped on odd frames so a skipped refit shows
//   6 compact             even bands instances of a compacted BLAS (COMPACTED_SIZE postbuild info, COPY_MODE_COMPACT)
//                         of a quad at z = 1.25, odd bands of a COPY_MODE_CLONE of the compacted copy of another quad
//                         at z = 1.75
//   7 compute-queue       tile 0's build recorded on a compute command list and executed on a compute queue, the direct
//                         queue waiting on its fence; a TLAS per frame slot
// Fail bits (dxr_as.hlsl): 1 a hit on an inactive band, 2 wrong hit distance (another BLAS), 4 InstanceIndex,
// 8 InstanceID, 16 miss on an active band. Tiles 1-3 are compiled with GFXRECON_TESTAPP_GPU_WRITTEN_TILES only:
// replayed without a translation that follows GPU writes, their builds read capture-time addresses, which can hang
// the GPU.

#include "d3d12_test_app.h"

#include "d3dx12/d3dx12.h"

#include "g_dxr_as_lib.h"
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
#include "g_dxr_as_write_descs_cs.h"
#include "g_dxr_as_write_pointers_cs.h"
#endif

#include <cstring>
#include <vector>

using namespace gfxrecon::testapp;

namespace
{

constexpr uint32_t kTilePointersCpu      = 0;
constexpr uint32_t kTilePointersGpu      = 1;
constexpr uint32_t kTileDescsGpu         = 2;
constexpr uint32_t kTilePointersGpuDescs = 3;
constexpr uint32_t kTilePointersInactive = 4;
constexpr uint32_t kTileUpdate           = 5;
constexpr uint32_t kTileCompact          = 6;
constexpr uint32_t kTileComputeQueue     = 7;
const char* const  kTileNames[]          = { "pointers-cpu",      "pointers-gpu", "descs-gpu", "pointers-gpu-descs",
                                             "pointers-inactive", "update",       "compact",   "compute-queue" };

constexpr uint32_t kBands         = 4;
constexpr uint32_t kAllBands      = 0xF;
constexpr uint32_t kInactiveBands = 0x6; // tile 4: bands 1 and 2
constexpr float    kZNear         = 1.0f;
constexpr float    kZFar          = 1.5f;
constexpr float    kZCompactEven  = 1.25f; // tile 6: the compacted chain of the even bands
constexpr float    kZCompactOdd   = 1.75f; // and the compacted-then-cloned chain of the odd bands
constexpr float    kQuadExtent    = 1.2f;
constexpr float    kBandScale     = 0.25f / kQuadExtent; // the quad's +-1.2 scaled to a band's +-0.25
constexpr float    kBandY0        = -0.75f;              // band 0's y offset; band b sits at kBandY0 + b * kBandDy
constexpr float    kBandDy        = 0.5f;
constexpr float    kOffTile       = 5.0f; // a band offset no ray reaches
constexpr uint32_t kDescBytes     = sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
static_assert(kDescBytes == 64, "main_write_descs assumes 64-byte descs");
constexpr uint32_t kPointerBytes = sizeof(D3D12_GPU_VIRTUAL_ADDRESS);

// The raygen shader's constants (dxr_as.hlsl BandConstants).
struct BandConstants
{
    uint32_t tile_x;
    uint32_t tile_y;
    uint32_t tile_size;
    uint32_t active_mask;
    float    z_even;
    float    z_odd;
    uint32_t pad0;
    uint32_t pad1;
};
constexpr UINT kBandConstantCount = sizeof(BandConstants) / sizeof(uint32_t);

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
constexpr bool kGpuWrittenTiles = true;

// The root constants of main_write_descs and main_write_pointers (dxr_as_compute.hlsl).
struct DescValues
{
    uint32_t blas_even[2];
    uint32_t blas_odd[2];
    uint32_t count;
    float    scale; // the band transform the shader writes: y scaled by `scale`, offset y0 + dy * band
    float    y0;
    float    dy;
};
struct PointerValues
{
    uint32_t pointers[kBands][2];
};
constexpr UINT kWriterConstantCount = sizeof(DescValues) / sizeof(uint32_t);
static_assert(sizeof(PointerValues) == sizeof(DescValues), "writer constants");
#else
constexpr bool kGpuWrittenTiles = false;
#endif

constexpr uint32_t kIdentifierBytes    = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
constexpr uint32_t kRaygenOffset       = 0;
constexpr uint32_t kMissOffset         = 64;
constexpr uint32_t kHitOffset          = 128;
constexpr uint32_t kTablesBytes        = kHitOffset + kIdentifierBytes;
const wchar_t      kRayGenExport[]     = L"RayGen";
const wchar_t      kClosestHitExport[] = L"ClosestHit";
const wchar_t      kMissExport[]       = L"Miss";
const wchar_t      kHitGroupExport[]   = L"HitGroup";

struct Vertex
{
    float x, y, z;
};

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
void SplitAddress(D3D12_GPU_VIRTUAL_ADDRESS address, uint32_t* out)
{
    out[0] = static_cast<uint32_t>(address);
    out[1] = static_cast<uint32_t>(address >> 32);
}
#endif

// The y offset that lands band b in its rows: the tile's [-1, 1] split in four.
float BandY(uint32_t band)
{
    return kBandY0 + kBandDy * static_cast<float>(band);
}

// An upload buffer kept mapped, rewritten by the CPU each frame.
struct MappedBuffer
{
    ComPtr<ID3D12Resource> buffer;
    void*                  mapped{ nullptr };
};

// One frame slot's CPU-written build inputs: a desc per band, a buffer each, and a pointer array over them.
struct BandInputs
{
    MappedBuffer descs[kBands];
    MappedBuffer pointers;
};

} // namespace

class DxrAccelerationStructuresApp : public D3d12TestApp
{
  protected:
    const char*    Name() const override { return "dxr-acceleration-structures"; }
    const wchar_t* WindowTitle() const override { return L"GFXR dxr-acceleration-structures test"; }
    uint32_t       TileCount() const override { return 8; }
    const char*    TileName(uint32_t i) const override { return kTileNames[i]; }
    bool           TileBuilt(uint32_t i) const override
    {
        return kGpuWrittenTiles || ((i != kTilePointersGpu) && (i != kTileDescsGpu) && (i != kTilePointersGpuDescs));
    }

    void Initialize() override;
    void RecordFrame(uint32_t frame, uint32_t slot) override;

  private:
    MappedBuffer CreateMappedBuffer(uint64_t size, const wchar_t* name);
    BandInputs   CreateBandInputs(const wchar_t* name, bool with_pointers);
    // The quad at depth z in a BLAS of its own, built on the open list.
    AccelerationStructure
    CreateQuadBlas(float z, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags, const wchar_t* name);
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    // A compute pipeline over {root constants at b<reg>, root UAV at u<reg>}.
    void CreateWriter(UINT                         reg,
                      D3D12_SHADER_BYTECODE        shader,
                      ComPtr<ID3D12RootSignature>* root_signature,
                      ComPtr<ID3D12PipelineState>* pipeline);
#endif

    // Band b's instance of `blas`: the quad scaled into a band's rows and moved to `ty` (BandY(b) for its own rows).
    D3D12_RAYTRACING_INSTANCE_DESC BandDesc(uint32_t band, D3D12_GPU_VIRTUAL_ADDRESS blas, float ty) const;
    // The BLAS of band b's parity: the near quad on even bands, the far one on odd.
    D3D12_GPU_VIRTUAL_ADDRESS BandBlas(uint32_t band) const;
    static D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS
    TlasInputs(D3D12_ELEMENTS_LAYOUT                               layout,
               D3D12_GPU_VIRTUAL_ADDRESS                           instance_descs,
               D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags =
                   D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE);

    // Writes the four descs of `inputs` (band b over BandBlas(b), no BLAS on the bands of `inactive_mask`) and, when
    // it has one, its pointer array.
    void WriteBandInputs(BandInputs& inputs, uint32_t inactive_mask = 0);
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    // Records main_write_descs / main_write_pointers into a default-heap buffer and leaves it readable by a build.
    void WriteDescsOnGpu(ID3D12Resource* descs);
    void WritePointersOnGpu(ID3D12Resource* pointers, const D3D12_GPU_VIRTUAL_ADDRESS* values);
#endif
    void DispatchTile(uint32_t tile, D3D12_GPU_VIRTUAL_ADDRESS tlas, uint32_t active_mask, float z_even, float z_odd);

    ComPtr<ID3D12Resource>              indices_;
    std::vector<ComPtr<ID3D12Resource>> vertex_buffers_;
    AccelerationStructure               blas_near_;
    AccelerationStructure               blas_far_;
    AccelerationStructure               blas_compaction_source_[2];
    ComPtr<ID3D12Resource>              blas_compacted_[2];
    ComPtr<ID3D12Resource>              blas_clone_;
    ComPtr<ID3D12Resource>              postbuild_info_;
    ComPtr<ID3D12Resource>              postbuild_readback_;
    ComPtr<ID3D12RootSignature>         root_signature_;
    ComPtr<ID3D12StateObject>           state_object_;
    ComPtr<ID3D12Resource>              tables_;
    ComPtr<ID3D12RootSignature>         write_descs_root_signature_;
    ComPtr<ID3D12PipelineState>         write_descs_pipeline_;
    ComPtr<ID3D12RootSignature>         write_pointers_root_signature_;
    ComPtr<ID3D12PipelineState>         write_pointers_pipeline_;

    AccelerationStructure  tlas_[7];                              // tiles 0-6; tile 7 has one per slot
    BandInputs             pointers_cpu_[kFramesInFlight];        // tile 0
    BandInputs             pointers_gpu_inputs_[kFramesInFlight]; // tile 1: the descs; the array is GPU-written
    ComPtr<ID3D12Resource> pointers_gpu_array_[kFramesInFlight];
    ComPtr<ID3D12Resource> descs_gpu_[kFramesInFlight];          // tile 2
    MappedBuffer           gpu_descs_pointers_[kFramesInFlight]; // tile 3: CPU pointers into GPU-written descs
    ComPtr<ID3D12Resource> gpu_descs_[kFramesInFlight];
    BandInputs             pointers_inactive_[kFramesInFlight]; // tile 4
    ComPtr<ID3D12Resource> update_init_descs_;                  // tile 5
    MappedBuffer           update_descs_[kFramesInFlight];
    MappedBuffer           compact_descs_[kFramesInFlight]; // tile 6

    ComPtr<ID3D12CommandQueue>         compute_queue_; // tile 7
    ComPtr<ID3D12CommandAllocator>     compute_allocators_[kFramesInFlight];
    ComPtr<ID3D12GraphicsCommandList4> compute_list_;
    ComPtr<ID3D12Fence>                compute_fence_;
    uint64_t                           compute_fence_value_{ 0 };
    AccelerationStructure              compute_tlas_[kFramesInFlight];
    BandInputs                         compute_inputs_[kFramesInFlight];
};

MappedBuffer DxrAccelerationStructuresApp::CreateMappedBuffer(uint64_t size, const wchar_t* name)
{
    MappedBuffer buffer;
    buffer.buffer       = CreateUploadBuffer(nullptr, size, name);
    D3D12_RANGE no_read = { 0, 0 };
    Check(buffer.buffer->Map(0, &no_read, &buffer.mapped), "Map (upload)");
    return buffer;
}

BandInputs DxrAccelerationStructuresApp::CreateBandInputs(const wchar_t* name, bool with_pointers)
{
    BandInputs inputs;
    for (uint32_t b = 0; b < kBands; ++b)
    {
        inputs.descs[b] = CreateMappedBuffer(kDescBytes, name);
    }
    if (with_pointers)
    {
        inputs.pointers = CreateMappedBuffer(kBands * kPointerBytes, name);
    }
    return inputs;
}

AccelerationStructure DxrAccelerationStructuresApp::CreateQuadBlas(
    float z, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags, const wchar_t* name)
{
    const Vertex           vertices[4] = { { -kQuadExtent, -kQuadExtent, z },
                                           { kQuadExtent, -kQuadExtent, z },
                                           { kQuadExtent, kQuadExtent, z },
                                           { -kQuadExtent, kQuadExtent, z } };
    ComPtr<ID3D12Resource> buffer      = CreateUploadBuffer(vertices, sizeof(vertices), L"quad vertices");
    vertex_buffers_.push_back(buffer);

    D3D12_RAYTRACING_GEOMETRY_DESC geometry       = {};
    geometry.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
    geometry.Flags                                = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
    geometry.Triangles.IndexBuffer                = indices_->GetGPUVirtualAddress();
    geometry.Triangles.IndexCount                 = 6;
    geometry.Triangles.IndexFormat                = DXGI_FORMAT_R16_UINT;
    geometry.Triangles.VertexFormat               = DXGI_FORMAT_R32G32B32_FLOAT;
    geometry.Triangles.VertexCount                = 4;
    geometry.Triangles.VertexBuffer.StartAddress  = buffer->GetGPUVirtualAddress();
    geometry.Triangles.VertexBuffer.StrideInBytes = sizeof(Vertex);

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
    inputs.Type              = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
    inputs.Flags             = flags;
    inputs.NumDescs          = 1;
    inputs.DescsLayout       = D3D12_ELEMENTS_LAYOUT_ARRAY;
    inputs.pGeometryDescs    = &geometry;
    AccelerationStructure as = CreateAccelerationStructure(inputs, name);
    BuildAccelerationStructure(as, inputs);
    return as;
}

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
void DxrAccelerationStructuresApp::CreateWriter(UINT                         reg,
                                                D3D12_SHADER_BYTECODE        shader,
                                                ComPtr<ID3D12RootSignature>* root_signature,
                                                ComPtr<ID3D12PipelineState>* pipeline)
{
    D3D12_ROOT_PARAMETER params[2]        = {};
    params[0].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    params[0].Constants                   = { reg, 0, kWriterConstantCount };
    params[1].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_UAV;
    params[1].Descriptor                  = { reg, 0 };
    D3D12_ROOT_SIGNATURE_DESC desc        = {};
    desc.NumParameters                    = 2;
    desc.pParameters                      = params;
    *root_signature                       = CreateRootSignature(desc);
    D3D12_COMPUTE_PIPELINE_STATE_DESC pso = {};
    pso.pRootSignature                    = root_signature->Get();
    pso.CS                                = shader;
    Check(Device()->CreateComputePipelineState(&pso, IID_PPV_ARGS(pipeline->ReleaseAndGetAddressOf())),
          "CreateComputePipelineState (writer)");
}
#endif

D3D12_RAYTRACING_INSTANCE_DESC
DxrAccelerationStructuresApp::BandDesc(uint32_t band, D3D12_GPU_VIRTUAL_ADDRESS blas, float ty) const
{
    D3D12_RAYTRACING_INSTANCE_DESC desc = {};
    desc.Transform[0][0]                = 1.0f;
    desc.Transform[1][1]                = kBandScale;
    desc.Transform[1][3]                = ty;
    desc.Transform[2][2]                = 1.0f;
    desc.InstanceID                     = band;
    desc.InstanceMask                   = 0xFF;
    desc.AccelerationStructure          = blas;
    return desc;
}

D3D12_GPU_VIRTUAL_ADDRESS DxrAccelerationStructuresApp::BandBlas(uint32_t band) const
{
    return ((band & 1) != 0) ? blas_far_.result->GetGPUVirtualAddress() : blas_near_.result->GetGPUVirtualAddress();
}

D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS
DxrAccelerationStructuresApp::TlasInputs(D3D12_ELEMENTS_LAYOUT                               layout,
                                         D3D12_GPU_VIRTUAL_ADDRESS                           instance_descs,
                                         D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags)
{
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
    inputs.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
    inputs.Flags         = flags;
    inputs.NumDescs      = kBands;
    inputs.DescsLayout   = layout;
    inputs.InstanceDescs = instance_descs;
    return inputs;
}

void DxrAccelerationStructuresApp::Initialize()
{
    const uint16_t indices[6] = { 0, 1, 2, 0, 2, 3 };
    indices_                  = CreateUploadBuffer(indices, sizeof(indices), L"quad indices");
    blas_near_ =
        CreateQuadBlas(kZNear, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE, L"BLAS near");
    blas_far_ =
        CreateQuadBlas(kZFar, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE, L"BLAS far");

    if (TileEnabled(kTileCompact))
    {
        // Tile 6: two quads built for compaction, at the depths of the even and the odd bands; their compacted sizes
        // read back now, then a compact copy of each and a clone of the odd one. The even bands reference the compacted
        // quad at kZCompactEven, the odd bands the clone at kZCompactOdd, so a reference landing on the other chain, or
        // on a near or far quad, fails with bit 2. A reference to a chain's live source, or to the clone's source where
        // the clone was meant, does not (same depth); a missing or invalid clone faults or misses.
        const float               source_z[2] = { kZCompactEven, kZCompactOdd };
        D3D12_GPU_VIRTUAL_ADDRESS sources[2];
        for (uint32_t i = 0; i < 2; ++i)
        {
            blas_compaction_source_[i] =
                CreateQuadBlas(source_z[i],
                               D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE |
                                   D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION,
                               L"BLAS compaction source");
            sources[i] = blas_compaction_source_[i].result->GetGPUVirtualAddress();
        }
        constexpr uint64_t kInfoBytes =
            2 * sizeof(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE_DESC);
        postbuild_info_                                                       = CreateBuffer(kInfoBytes,
                                       D3D12_HEAP_TYPE_DEFAULT,
                                       D3D12_RESOURCE_STATE_COMMON,
                                       D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                       L"postbuild info");
        postbuild_readback_                                                   = CreateBuffer(kInfoBytes,
                                           D3D12_HEAP_TYPE_READBACK,
                                           D3D12_RESOURCE_STATE_COPY_DEST,
                                           D3D12_RESOURCE_FLAG_NONE,
                                           L"postbuild readback");
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC info_desc = {};
        info_desc.DestBuffer                                                  = postbuild_info_->GetGPUVirtualAddress();
        info_desc.InfoType = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE;
        Transition(postbuild_info_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
        List()->EmitRaytracingAccelerationStructurePostbuildInfo(&info_desc, 2, sources);
        Transition(postbuild_info_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
        List()->CopyBufferRegion(postbuild_readback_.Get(), 0, postbuild_info_.Get(), 0, kInfoBytes);
        ExecuteAndWait();
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE_DESC compacted[2] = {};
        {
            void*       mapped     = nullptr;
            D3D12_RANGE read_range = { 0, static_cast<SIZE_T>(kInfoBytes) };
            Check(postbuild_readback_->Map(0, &read_range, &mapped), "Map (postbuild readback)");
            std::memcpy(compacted, mapped, sizeof(compacted));
            D3D12_RANGE no_write = { 0, 0 };
            postbuild_readback_->Unmap(0, &no_write);
        }
        for (uint32_t i = 0; i < 2; ++i)
        {
            if (compacted[i].CompactedSizeInBytes == 0)
            {
                Check(E_FAIL, "compacted size");
            }
            blas_compacted_[i] = CreateBuffer(compacted[i].CompactedSizeInBytes,
                                              D3D12_HEAP_TYPE_DEFAULT,
                                              D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
                                              D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                              L"BLAS compacted");
            List()->CopyRaytracingAccelerationStructure(blas_compacted_[i]->GetGPUVirtualAddress(),
                                                        sources[i],
                                                        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_COMPACT);
            UavBarrier(blas_compacted_[i].Get());
        }
        blas_clone_ = CreateBuffer(compacted[1].CompactedSizeInBytes,
                                   D3D12_HEAP_TYPE_DEFAULT,
                                   D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
                                   D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                   L"BLAS clone");
        List()->CopyRaytracingAccelerationStructure(blas_clone_->GetGPUVirtualAddress(),
                                                    blas_compacted_[1]->GetGPUVirtualAddress(),
                                                    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_CLONE);
        UavBarrier(blas_clone_.Get());
    }

    // The pipeline: band constants, the output UAV, the TLAS as a root SRV; a hit group without local arguments.
    D3D12_DESCRIPTOR_RANGE uav_range = {
        D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    };
    D3D12_ROOT_PARAMETER params[3]        = {};
    params[0].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    params[0].Constants                   = { 0, 0, kBandConstantCount };
    params[1].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    params[1].DescriptorTable             = { 1, &uav_range };
    params[2].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_SRV;
    params[2].Descriptor                  = { 0, 0 };
    D3D12_ROOT_SIGNATURE_DESC global_desc = {};
    global_desc.NumParameters             = 3;
    global_desc.pParameters               = params;
    root_signature_                       = CreateRootSignature(global_desc);

    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    auto                      library = pipeline.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
    D3D12_SHADER_BYTECODE     code    = { g_dxr_as_lib, sizeof(g_dxr_as_lib) };
    library->SetDXILLibrary(&code);
    library->DefineExport(kRayGenExport);
    library->DefineExport(kClosestHitExport);
    library->DefineExport(kMissExport);
    auto hit_group = pipeline.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
    hit_group->SetClosestHitShaderImport(kClosestHitExport);
    hit_group->SetHitGroupExport(kHitGroupExport);
    hit_group->SetHitGroupType(D3D12_HIT_GROUP_TYPE_TRIANGLES);
    auto shader_config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
    shader_config->Config(4 * sizeof(uint32_t), 2 * sizeof(float));
    auto global = pipeline.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
    global->SetRootSignature(root_signature_.Get());
    auto pipeline_config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
    pipeline_config->Config(1);
    Check(Device()->CreateStateObject(pipeline, IID_PPV_ARGS(&state_object_)), "CreateStateObject");

    ComPtr<ID3D12StateObjectProperties> properties;
    Check(state_object_.As(&properties), "QueryInterface ID3D12StateObjectProperties");
    const void* raygen_id = properties->GetShaderIdentifier(kRayGenExport);
    const void* miss_id   = properties->GetShaderIdentifier(kMissExport);
    const void* hit_id    = properties->GetShaderIdentifier(kHitGroupExport);
    if ((raygen_id == nullptr) || (miss_id == nullptr) || (hit_id == nullptr))
    {
        Check(E_FAIL, "GetShaderIdentifier");
    }
    ShaderTable raygen(kIdentifierBytes);
    raygen.AddRecord(raygen_id);
    ShaderTable miss(kIdentifierBytes);
    miss.AddRecord(miss_id);
    ShaderTable hit(kIdentifierBytes);
    hit.AddRecord(hit_id);
    std::vector<uint8_t> bytes(kTablesBytes, 0);
    std::memcpy(bytes.data() + kRaygenOffset, raygen.Bytes().data(), raygen.Size());
    std::memcpy(bytes.data() + kMissOffset, miss.Bytes().data(), miss.Size());
    std::memcpy(bytes.data() + kHitOffset, hit.Bytes().data(), hit.Size());
    tables_ = CreateDefaultBuffer(bytes.data(),
                                  bytes.size(),
                                  D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
                                  D3D12_RESOURCE_FLAG_NONE,
                                  L"shader tables");

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    if (TileEnabled(kTilePointersGpu) || TileEnabled(kTileDescsGpu) || TileEnabled(kTilePointersGpuDescs))
    {
        CreateWriter(0,
                     { g_dxr_as_write_descs_cs, sizeof(g_dxr_as_write_descs_cs) },
                     &write_descs_root_signature_,
                     &write_descs_pipeline_);
        CreateWriter(1,
                     { g_dxr_as_write_pointers_cs, sizeof(g_dxr_as_write_pointers_cs) },
                     &write_pointers_root_signature_,
                     &write_pointers_pipeline_);
    }
#endif

    // The per-slot inputs of the tiles that rewrite them each frame; a TLAS per tile, sized for four instances.
    for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
    {
        if (TileEnabled(kTilePointersCpu))
        {
            pointers_cpu_[slot] = CreateBandInputs(L"pointers-cpu inputs", true);
        }
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
        if (TileEnabled(kTilePointersGpu))
        {
            pointers_gpu_inputs_[slot] = CreateBandInputs(L"pointers-gpu descs", false);
            pointers_gpu_array_[slot]  = CreateBuffer(kBands * kPointerBytes,
                                                     D3D12_HEAP_TYPE_DEFAULT,
                                                     D3D12_RESOURCE_STATE_COMMON,
                                                     D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                                     L"pointers-gpu array");
        }
        if (TileEnabled(kTileDescsGpu))
        {
            descs_gpu_[slot] = CreateBuffer(kBands * kDescBytes,
                                            D3D12_HEAP_TYPE_DEFAULT,
                                            D3D12_RESOURCE_STATE_COMMON,
                                            D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                            L"descs-gpu descs");
        }
        if (TileEnabled(kTilePointersGpuDescs))
        {
            gpu_descs_pointers_[slot] = CreateMappedBuffer(kBands * kPointerBytes, L"pointers-gpu-descs pointers");
            gpu_descs_[slot]          = CreateBuffer(kBands * kDescBytes,
                                            D3D12_HEAP_TYPE_DEFAULT,
                                            D3D12_RESOURCE_STATE_COMMON,
                                            D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                            L"pointers-gpu-descs descs");
        }
#endif
        if (TileEnabled(kTilePointersInactive))
        {
            pointers_inactive_[slot] = CreateBandInputs(L"pointers-inactive inputs", true);
        }
        if (TileEnabled(kTileUpdate))
        {
            update_descs_[slot] = CreateMappedBuffer(kBands * kDescBytes, L"update descs");
        }
        if (TileEnabled(kTileCompact))
        {
            compact_descs_[slot] = CreateMappedBuffer(kBands * kDescBytes, L"compact descs");
        }
        if (TileEnabled(kTileComputeQueue))
        {
            compute_inputs_[slot] = CreateBandInputs(L"compute-queue inputs", true);
            compute_tlas_[slot] =
                CreateAccelerationStructure(TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS,
                                                       compute_inputs_[slot].pointers.buffer->GetGPUVirtualAddress()),
                                            L"TLAS compute-queue");
        }
    }
    if (TileEnabled(kTilePointersCpu))
    {
        tlas_[kTilePointersCpu] =
            CreateAccelerationStructure(TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS,
                                                   pointers_cpu_[0].pointers.buffer->GetGPUVirtualAddress()),
                                        L"TLAS pointers-cpu");
    }
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    if (TileEnabled(kTilePointersGpu))
    {
        tlas_[kTilePointersGpu] = CreateAccelerationStructure(
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS, pointers_gpu_array_[0]->GetGPUVirtualAddress()),
            L"TLAS pointers-gpu");
    }
    if (TileEnabled(kTileDescsGpu))
    {
        tlas_[kTileDescsGpu] = CreateAccelerationStructure(
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY, descs_gpu_[0]->GetGPUVirtualAddress()), L"TLAS descs-gpu");
    }
    if (TileEnabled(kTilePointersGpuDescs))
    {
        tlas_[kTilePointersGpuDescs] = CreateAccelerationStructure(
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS, gpu_descs_pointers_[0].buffer->GetGPUVirtualAddress()),
            L"TLAS pointers-gpu-descs");
    }
#endif
    if (TileEnabled(kTilePointersInactive))
    {
        tlas_[kTilePointersInactive] =
            CreateAccelerationStructure(TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS,
                                                   pointers_inactive_[0].pointers.buffer->GetGPUVirtualAddress()),
                                        L"TLAS pointers-inactive");
    }
    if (TileEnabled(kTileCompact))
    {
        tlas_[kTileCompact] = CreateAccelerationStructure(
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY, compact_descs_[0].buffer->GetGPUVirtualAddress()), L"TLAS compact");
    }

    if (TileEnabled(kTileUpdate))
    {
        // Tile 5: built now, updatable, every instance off the tile; the frames refit it into the bands.
        D3D12_RAYTRACING_INSTANCE_DESC off_tile[kBands];
        for (uint32_t b = 0; b < kBands; ++b)
        {
            off_tile[b] = BandDesc(b, BandBlas(b), kOffTile);
        }
        update_init_descs_ = CreateUploadBuffer(off_tile, sizeof(off_tile), L"update init descs");
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS update_inputs =
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY,
                       update_init_descs_->GetGPUVirtualAddress(),
                       D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE |
                           D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE);
        tlas_[kTileUpdate] = CreateAccelerationStructure(update_inputs, L"TLAS update");
        BuildAccelerationStructure(tlas_[kTileUpdate], update_inputs);
    }

    if (TileEnabled(kTileComputeQueue))
    {
        // Tile 7: the compute queue, its allocators, list and fence.
        D3D12_COMMAND_QUEUE_DESC queue_desc = {};
        queue_desc.Type                     = D3D12_COMMAND_LIST_TYPE_COMPUTE;
        Check(Device()->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&compute_queue_)), "CreateCommandQueue (compute)");
        for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
        {
            Check(Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE,
                                                   IID_PPV_ARGS(&compute_allocators_[slot])),
                  "CreateCommandAllocator (compute)");
        }
        Check(Device()->CreateCommandList(0,
                                          D3D12_COMMAND_LIST_TYPE_COMPUTE,
                                          compute_allocators_[0].Get(),
                                          nullptr,
                                          IID_PPV_ARGS(&compute_list_)),
              "CreateCommandList (compute)");
        Check(compute_list_->Close(), "Close (compute)");
        Check(Device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&compute_fence_)), "CreateFence (compute)");
    }
}

void DxrAccelerationStructuresApp::WriteBandInputs(BandInputs& inputs, uint32_t inactive_mask)
{
    D3D12_GPU_VIRTUAL_ADDRESS pointers[kBands];
    for (uint32_t b = 0; b < kBands; ++b)
    {
        D3D12_RAYTRACING_INSTANCE_DESC desc = BandDesc(b, BandBlas(b), BandY(b));
        if ((inactive_mask & (1u << b)) != 0)
        {
            desc.AccelerationStructure = 0;
        }
        std::memcpy(inputs.descs[b].mapped, &desc, sizeof(desc));
        pointers[b] = inputs.descs[b].buffer->GetGPUVirtualAddress();
    }
    if (inputs.pointers.mapped != nullptr)
    {
        std::memcpy(inputs.pointers.mapped, pointers, sizeof(pointers));
    }
}

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
void DxrAccelerationStructuresApp::WriteDescsOnGpu(ID3D12Resource* descs)
{
    ID3D12GraphicsCommandList4* list   = List();
    DescValues                  values = {};
    SplitAddress(blas_near_.result->GetGPUVirtualAddress(), values.blas_even);
    SplitAddress(blas_far_.result->GetGPUVirtualAddress(), values.blas_odd);
    values.count = kBands;
    values.scale = kBandScale;
    values.y0    = kBandY0;
    values.dy    = kBandDy;
    list->SetComputeRootSignature(write_descs_root_signature_.Get());
    list->SetPipelineState(write_descs_pipeline_.Get());
    list->SetComputeRoot32BitConstants(0, kWriterConstantCount, &values, 0);
    list->SetComputeRootUnorderedAccessView(1, descs->GetGPUVirtualAddress());
    list->Dispatch(1, 1, 1);
    UavBarrier(descs);
    Transition(descs, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
}

void DxrAccelerationStructuresApp::WritePointersOnGpu(ID3D12Resource* pointers, const D3D12_GPU_VIRTUAL_ADDRESS* values)
{
    ID3D12GraphicsCommandList4* list      = List();
    PointerValues               constants = {};
    for (uint32_t b = 0; b < kBands; ++b)
    {
        SplitAddress(values[b], constants.pointers[b]);
    }
    list->SetComputeRootSignature(write_pointers_root_signature_.Get());
    list->SetPipelineState(write_pointers_pipeline_.Get());
    list->SetComputeRoot32BitConstants(0, kWriterConstantCount, &constants, 0);
    list->SetComputeRootUnorderedAccessView(1, pointers->GetGPUVirtualAddress());
    list->Dispatch(1, 1, 1);
    UavBarrier(pointers);
    Transition(pointers, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
}
#endif

void DxrAccelerationStructuresApp::RecordFrame(uint32_t frame, uint32_t slot)
{
    ID3D12GraphicsCommandList4* list = List();
    // Tile 5's refit alternates the bands' depths, so the TLAS a trim's state block restores from one frame never
    // passes the next frame's check: a replay that skips the refit sets bit 2 every other frame.
    const bool swap_depths = (frame & 1) != 0;

    // The rebuilds wait for the previous frame's rays over the same structures and its use of the scratch buffers.
    UavBarrier();

    if (TileEnabled(kTilePointersCpu))
    {
        BandInputs& inputs = pointers_cpu_[slot];
        WriteBandInputs(inputs);
        BuildAccelerationStructure(
            tlas_[kTilePointersCpu],
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS, inputs.pointers.buffer->GetGPUVirtualAddress()));
    }
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    if (TileEnabled(kTilePointersGpu))
    {
        BandInputs& inputs = pointers_gpu_inputs_[slot];
        WriteBandInputs(inputs);
        D3D12_GPU_VIRTUAL_ADDRESS values[kBands];
        for (uint32_t b = 0; b < kBands; ++b)
        {
            values[b] = inputs.descs[b].buffer->GetGPUVirtualAddress();
        }
        WritePointersOnGpu(pointers_gpu_array_[slot].Get(), values);
        BuildAccelerationStructure(
            tlas_[kTilePointersGpu],
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS, pointers_gpu_array_[slot]->GetGPUVirtualAddress()));
    }
    if (TileEnabled(kTileDescsGpu))
    {
        WriteDescsOnGpu(descs_gpu_[slot].Get());
        BuildAccelerationStructure(tlas_[kTileDescsGpu],
                                   TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY, descs_gpu_[slot]->GetGPUVirtualAddress()));
    }
    if (TileEnabled(kTilePointersGpuDescs))
    {
        WriteDescsOnGpu(gpu_descs_[slot].Get());
        D3D12_GPU_VIRTUAL_ADDRESS pointers[kBands];
        for (uint32_t b = 0; b < kBands; ++b)
        {
            pointers[b] = gpu_descs_[slot]->GetGPUVirtualAddress() + b * kDescBytes;
        }
        std::memcpy(gpu_descs_pointers_[slot].mapped, pointers, sizeof(pointers));
        BuildAccelerationStructure(tlas_[kTilePointersGpuDescs],
                                   TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS,
                                              gpu_descs_pointers_[slot].buffer->GetGPUVirtualAddress()));
    }
#endif
    if (TileEnabled(kTilePointersInactive))
    {
        BandInputs& inputs = pointers_inactive_[slot];
        WriteBandInputs(inputs, kInactiveBands);
        BuildAccelerationStructure(
            tlas_[kTilePointersInactive],
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS, inputs.pointers.buffer->GetGPUVirtualAddress()));
    }
    if (TileEnabled(kTileUpdate))
    {
        D3D12_RAYTRACING_INSTANCE_DESC descs[kBands];
        for (uint32_t b = 0; b < kBands; ++b)
        {
            descs[b] = BandDesc(b, BandBlas(swap_depths ? b + 1 : b), BandY(b));
        }
        std::memcpy(update_descs_[slot].mapped, descs, sizeof(descs));
        BuildAccelerationStructure(tlas_[kTileUpdate],
                                   TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY,
                                              update_descs_[slot].buffer->GetGPUVirtualAddress(),
                                              D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE |
                                                  D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE |
                                                  D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE),
                                   tlas_[kTileUpdate].result->GetGPUVirtualAddress());
    }
    if (TileEnabled(kTileCompact))
    {
        D3D12_RAYTRACING_INSTANCE_DESC descs[kBands];
        for (uint32_t b = 0; b < kBands; ++b)
        {
            ID3D12Resource* blas = ((b & 1) != 0) ? blas_clone_.Get() : blas_compacted_[0].Get();
            descs[b]             = BandDesc(b, blas->GetGPUVirtualAddress(), BandY(b));
        }
        std::memcpy(compact_descs_[slot].mapped, descs, sizeof(descs));
        BuildAccelerationStructure(
            tlas_[kTileCompact],
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY, compact_descs_[slot].buffer->GetGPUVirtualAddress()));
    }
    if (TileEnabled(kTileComputeQueue))
    {
        // This slot's structure was last read two frames ago, which the frame loop waited for; the direct queue waits
        // for this build before the frame's rays.
        BandInputs& inputs = compute_inputs_[slot];
        WriteBandInputs(inputs);
        Check(compute_allocators_[slot]->Reset(), "CommandAllocator::Reset (compute)");
        Check(compute_list_->Reset(compute_allocators_[slot].Get(), nullptr), "CommandList::Reset (compute)");
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc = {};
        desc.Inputs =
            TlasInputs(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS, inputs.pointers.buffer->GetGPUVirtualAddress());
        desc.DestAccelerationStructureData    = compute_tlas_[slot].result->GetGPUVirtualAddress();
        desc.ScratchAccelerationStructureData = compute_tlas_[slot].scratch->GetGPUVirtualAddress();
        compute_list_->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_UAV;
        barrier.UAV.pResource          = compute_tlas_[slot].result.Get();
        compute_list_->ResourceBarrier(1, &barrier);
        Check(compute_list_->Close(), "Close (compute)");
        ID3D12CommandList* lists[] = { compute_list_.Get() };
        compute_queue_->ExecuteCommandLists(1, lists);
        Check(compute_queue_->Signal(compute_fence_.Get(), ++compute_fence_value_), "Signal (compute)");
        WaitBefore(compute_fence_.Get(), compute_fence_value_);
    }

    list->SetComputeRootSignature(root_signature_.Get());
    list->SetComputeRootDescriptorTable(1, OutputUav().gpu);
    list->SetPipelineState1(state_object_.Get());
    if (TileEnabled(kTilePointersCpu))
    {
        DispatchTile(
            kTilePointersCpu, tlas_[kTilePointersCpu].result->GetGPUVirtualAddress(), kAllBands, kZNear, kZFar);
    }
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    if (TileEnabled(kTilePointersGpu))
    {
        DispatchTile(
            kTilePointersGpu, tlas_[kTilePointersGpu].result->GetGPUVirtualAddress(), kAllBands, kZNear, kZFar);
    }
    if (TileEnabled(kTileDescsGpu))
    {
        DispatchTile(kTileDescsGpu, tlas_[kTileDescsGpu].result->GetGPUVirtualAddress(), kAllBands, kZNear, kZFar);
    }
    if (TileEnabled(kTilePointersGpuDescs))
    {
        DispatchTile(kTilePointersGpuDescs,
                     tlas_[kTilePointersGpuDescs].result->GetGPUVirtualAddress(),
                     kAllBands,
                     kZNear,
                     kZFar);
    }
#endif
    if (TileEnabled(kTilePointersInactive))
    {
        DispatchTile(kTilePointersInactive,
                     tlas_[kTilePointersInactive].result->GetGPUVirtualAddress(),
                     kAllBands & ~kInactiveBands,
                     kZNear,
                     kZFar);
    }
    if (TileEnabled(kTileUpdate))
    {
        DispatchTile(kTileUpdate,
                     tlas_[kTileUpdate].result->GetGPUVirtualAddress(),
                     kAllBands,
                     swap_depths ? kZFar : kZNear,
                     swap_depths ? kZNear : kZFar);
    }
    if (TileEnabled(kTileCompact))
    {
        DispatchTile(
            kTileCompact, tlas_[kTileCompact].result->GetGPUVirtualAddress(), kAllBands, kZCompactEven, kZCompactOdd);
    }
    if (TileEnabled(kTileComputeQueue))
    {
        DispatchTile(kTileComputeQueue, compute_tlas_[slot].result->GetGPUVirtualAddress(), kAllBands, kZNear, kZFar);
    }
}

void DxrAccelerationStructuresApp::DispatchTile(
    uint32_t tile, D3D12_GPU_VIRTUAL_ADDRESS tlas, uint32_t active_mask, float z_even, float z_odd)
{
    ID3D12GraphicsCommandList4* list      = List();
    BandConstants               constants = { TileX(tile), TileY(tile), kTileSize, active_mask, z_even, z_odd, 0, 0 };
    list->SetComputeRoot32BitConstants(0, kBandConstantCount, &constants, 0);
    list->SetComputeRootShaderResourceView(2, tlas);

    D3D12_GPU_VIRTUAL_ADDRESS base              = tables_->GetGPUVirtualAddress();
    D3D12_DISPATCH_RAYS_DESC  desc              = {};
    desc.RayGenerationShaderRecord.StartAddress = base + kRaygenOffset;
    desc.RayGenerationShaderRecord.SizeInBytes  = kIdentifierBytes;
    desc.MissShaderTable.StartAddress           = base + kMissOffset;
    desc.MissShaderTable.SizeInBytes            = kIdentifierBytes;
    desc.MissShaderTable.StrideInBytes          = kIdentifierBytes;
    desc.HitGroupTable.StartAddress             = base + kHitOffset;
    desc.HitGroupTable.SizeInBytes              = kIdentifierBytes;
    desc.HitGroupTable.StrideInBytes            = kIdentifierBytes;
    desc.Width                                  = kTileSize;
    desc.Height                                 = kTileSize;
    desc.Depth                                  = 1;
    list->DispatchRays(&desc);
}

GFXRECON_D3D12_TEST_APP_MAIN(DxrAccelerationStructuresApp)
