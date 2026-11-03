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

// dxr-command-contexts: the command contexts a DispatchRays or an acceleration structure build can reach the GPU
// from, on the shared scene (dxr_scene.h). Every tile dispatches the scene's pipeline over a TLAS of the scene's
// quad, so a context the replay fails to follow shows as a red or black tile, or a fault. Tiles:
//   0 bundle-dispatch    DispatchRays recorded in a bundle, executed by the frame's list
//   1 bundle-indirect    ExecuteIndirect with a DISPATCH_RAYS signature recorded in a bundle
//   2 compute-queue      DispatchRays on a compute command list executed on a compute queue, into a texture of its
//                        own that the frame's list copies into the tile after a fence wait
//   3 copy-queue         the instance desc the CPU wrote copied by a copy queue into the default-heap buffer the
//                        frame's TLAS build reads, behind a fence
//   4 enhanced-barriers  the shader table copied into place and the TLAS rebuilt every frame with
//                        ID3D12GraphicsCommandList7::Barrier only, no legacy ResourceBarrier
//   5 cross-list-build   the TLAS built on a list submitted ahead of the frame's list in the same
//                        ExecuteCommandLists, dispatched over by the frame's list
//   6 reused-list        a command list recorded once at initialization and executed every frame without a reset,
//                        into a texture of its own
// Tiles 0 and 1 are compiled with GFXRECON_TESTAPP_KNOWN_DEFECT_TILES only: Dx12ResourceValueMapper maps the records
// of the command lists a queue executes and never a bundle's, so their shader tables stay untranslated. Render passes
// have no tile: the render pass spec lists Dispatch* among the calls a pass forbids.
// Fail bits (dxr_scene.hlsl): 1 root constant, 2 root CBV, 4 descriptor table, 8 InstanceID, 16 miss.

#include "dxr_scene.h"

#include "d3dx12/d3dx12.h"

#include <cstring>
#include <vector>

using namespace gfxrecon::testapp;

namespace
{

constexpr uint32_t kTileBundleDispatch    = 0;
constexpr uint32_t kTileBundleIndirect    = 1;
constexpr uint32_t kTileComputeQueue      = 2;
constexpr uint32_t kTileCopyQueue         = 3;
constexpr uint32_t kTileEnhancedBarriers  = 4;
constexpr uint32_t kTileCrossListBuild    = 5;
constexpr uint32_t kTileReusedList        = 6;
constexpr uint32_t kTileCount             = 7;
const char* const  kTileNames[kTileCount] = { "bundle-dispatch",   "bundle-indirect",  "compute-queue", "copy-queue",
                                              "enhanced-barriers", "cross-list-build", "reused-list" };

#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
constexpr bool kDefectTiles = true;
#else
constexpr bool kDefectTiles = false;
#endif

constexpr uint32_t kIdentifierBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
constexpr uint32_t kRecordStride    = 64;
constexpr uint32_t kMissOffset      = 64;
constexpr uint32_t kHitOffset       = 128;
constexpr uint32_t kTablesBytes     = kHitOffset + kRecordStride;
constexpr uint32_t kDescBytes       = sizeof(D3D12_RAYTRACING_INSTANCE_DESC);

// The hit record's arguments in the scene's local root signature order (dxr_scene.cpp).
struct HitArguments
{
    uint32_t                    constant;
    uint32_t                    pad;
    D3D12_GPU_VIRTUAL_ADDRESS   cbv;
    D3D12_GPU_DESCRIPTOR_HANDLE table;
};
static_assert(sizeof(HitArguments) == 24, "hit record layout");

// A 128x128 UAV texture a context other than the frame's list draws into; the frame's list copies it into the tile.
struct TileTexture
{
    ComPtr<ID3D12Resource> texture;
    DescriptorHandle       uav;
};

// An upload buffer kept mapped, rewritten by the CPU each frame.
struct MappedBuffer
{
    ComPtr<ID3D12Resource> buffer;
    void*                  mapped{ nullptr };
};

} // namespace

class DxrCommandContextsApp : public D3d12TestApp
{
  protected:
    const char*    Name() const override { return "dxr-command-contexts"; }
    const wchar_t* WindowTitle() const override { return L"GFXR dxr-command-contexts test"; }
    uint32_t       TileCount() const override { return kTileCount; }
    const char*    TileName(uint32_t i) const override { return kTileNames[i]; }
    bool           TileBuilt(uint32_t i) const override
    {
        return kDefectTiles || ((i != kTileBundleDispatch) && (i != kTileBundleIndirect));
    }

    void Initialize() override;
    void RecordFrame(uint32_t frame, uint32_t slot) override;

  private:
    MappedBuffer CreateMappedBuffer(uint64_t size, const wchar_t* name);
    TileTexture  CreateTileTexture(const wchar_t* name);
    // The scene's root signature and pipeline bound on `list` (whose descriptor heap is set), the tile constants at
    // (x, y) with instance A expected, the output UAV and the TLAS, then the dispatch of `desc`.
    void BindAndDispatch(ID3D12GraphicsCommandList4*     list,
                         uint32_t                        x,
                         uint32_t                        y,
                         D3D12_GPU_DESCRIPTOR_HANDLE     output_uav,
                         D3D12_GPU_VIRTUAL_ADDRESS       tlas,
                         const D3D12_DISPATCH_RAYS_DESC& desc);
    // The frame's list copies a tile texture into its tile of the output.
    void CopyTile(uint32_t tile, const TileTexture& texture);
    // A top-level build of one instance desc at `instance`, recorded on `list` without the common library's barrier.
    void
    RecordBuild(ID3D12GraphicsCommandList4* list, const AccelerationStructure& as, D3D12_GPU_VIRTUAL_ADDRESS instance);
    void BufferBarrier(ID3D12Resource*      buffer,
                       D3D12_BARRIER_SYNC   sync_before,
                       D3D12_BARRIER_SYNC   sync_after,
                       D3D12_BARRIER_ACCESS access_before,
                       D3D12_BARRIER_ACCESS access_after);
    void GlobalBarrier(D3D12_BARRIER_SYNC   sync_before,
                       D3D12_BARRIER_SYNC   sync_after,
                       D3D12_BARRIER_ACCESS access_before,
                       D3D12_BARRIER_ACCESS access_after);
    // The scene's dispatch desc over tables at `base` laid out as the scene's (raygen 0, miss 64, hit group 128).
    D3D12_DISPATCH_RAYS_DESC DescOver(D3D12_GPU_VIRTUAL_ADDRESS base) const;

    void CreateComputeQueue();
    void CreateCopyQueue();
    void CreateEnhancedBarriers();
    void CreateCrossListBuild();
    void CreateReusedList();
#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
    void CreateBundles();
#endif

    DxrScene scene_;

    ComPtr<ID3D12CommandAllocator>     bundle_allocator_; // tiles 0 and 1
    ComPtr<ID3D12GraphicsCommandList4> bundle_dispatch_;
    ComPtr<ID3D12GraphicsCommandList4> bundle_indirect_;
    ComPtr<ID3D12CommandSignature>     dispatch_signature_;
    ComPtr<ID3D12Resource>             indirect_args_;

    ComPtr<ID3D12CommandQueue>         compute_queue_; // tile 2
    ComPtr<ID3D12CommandAllocator>     compute_allocators_[kFramesInFlight];
    ComPtr<ID3D12GraphicsCommandList4> compute_list_;
    ComPtr<ID3D12Fence>                compute_fence_;
    uint64_t                           compute_fence_value_{ 0 };
    TileTexture                        compute_texture_[kFramesInFlight];

    ComPtr<ID3D12CommandQueue>        copy_queue_; // tile 3
    ComPtr<ID3D12CommandAllocator>    copy_allocators_[kFramesInFlight];
    ComPtr<ID3D12GraphicsCommandList> copy_list_;
    ComPtr<ID3D12Fence>               copy_fence_;
    uint64_t                          copy_fence_value_{ 0 };
    MappedBuffer                      copy_instance_upload_[kFramesInFlight];
    ComPtr<ID3D12Resource>            copy_instance_default_[kFramesInFlight];
    AccelerationStructure             copy_tlas_;

    ComPtr<ID3D12GraphicsCommandList7> list7_; // tile 4
    ComPtr<ID3D12Resource>             barrier_tables_upload_;
    ComPtr<ID3D12Resource>             barrier_tables_;
    MappedBuffer                       barrier_instance_[kFramesInFlight];
    AccelerationStructure              barrier_tlas_;

    ComPtr<ID3D12CommandAllocator>     cross_allocators_[kFramesInFlight]; // tile 5
    ComPtr<ID3D12GraphicsCommandList4> cross_list_;
    MappedBuffer                       cross_instance_[kFramesInFlight];
    AccelerationStructure              cross_tlas_;

    ComPtr<ID3D12CommandAllocator>     reused_allocator_; // tile 6
    ComPtr<ID3D12GraphicsCommandList4> reused_list_;
    TileTexture                        reused_texture_;
};

MappedBuffer DxrCommandContextsApp::CreateMappedBuffer(uint64_t size, const wchar_t* name)
{
    MappedBuffer buffer;
    buffer.buffer       = CreateUploadBuffer(nullptr, size, name);
    D3D12_RANGE no_read = { 0, 0 };
    Check(buffer.buffer->Map(0, &no_read, &buffer.mapped), "Map (upload)");
    return buffer;
}

TileTexture DxrCommandContextsApp::CreateTileTexture(const wchar_t* name)
{
    TileTexture           tile;
    D3D12_HEAP_PROPERTIES heap = {};
    heap.Type                  = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC desc   = {};
    desc.Dimension             = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width                 = kTileSize;
    desc.Height                = kTileSize;
    desc.DepthOrArraySize      = 1;
    desc.MipLevels             = 1;
    desc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count      = 1;
    desc.Flags                 = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    Check(Device()->CreateCommittedResource(&heap,
                                            D3D12_HEAP_FLAG_NONE,
                                            &desc,
                                            D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                            nullptr,
                                            IID_PPV_ARGS(&tile.texture)),
          "CreateCommittedResource (tile texture)");
    tile.texture->SetName(name);
    tile.uav                              = AllocateDescriptor();
    D3D12_UNORDERED_ACCESS_VIEW_DESC view = {};
    view.Format                           = DXGI_FORMAT_R8G8B8A8_UNORM;
    view.ViewDimension                    = D3D12_UAV_DIMENSION_TEXTURE2D;
    Device()->CreateUnorderedAccessView(tile.texture.Get(), nullptr, &view, tile.uav.cpu);
    return tile;
}

void DxrCommandContextsApp::BindAndDispatch(ID3D12GraphicsCommandList4*     list,
                                            uint32_t                        x,
                                            uint32_t                        y,
                                            D3D12_GPU_DESCRIPTOR_HANDLE     output_uav,
                                            D3D12_GPU_VIRTUAL_ADDRESS       tlas,
                                            const D3D12_DISPATCH_RAYS_DESC& desc)
{
    TileConstants constants = { x, y, kTileSize, kInstanceIdA };
    list->SetComputeRootSignature(scene_.global_root_signature.Get());
    list->SetComputeRoot32BitConstants(0, kTileConstantCount, &constants, 0);
    list->SetComputeRootDescriptorTable(1, output_uav);
    list->SetComputeRootShaderResourceView(2, tlas);
    list->SetPipelineState1(scene_.state_object.Get());
    list->DispatchRays(&desc);
}

void DxrCommandContextsApp::CopyTile(uint32_t tile, const TileTexture& texture)
{
    Transition(texture.texture.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
    Transition(Output(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_DEST);
    CD3DX12_TEXTURE_COPY_LOCATION dst(Output(), 0);
    CD3DX12_TEXTURE_COPY_LOCATION src(texture.texture.Get(), 0);
    D3D12_BOX                     box = { 0, 0, 0, kTileSize, kTileSize, 1 };
    List()->CopyTextureRegion(&dst, TileX(tile), TileY(tile), 0, &src, &box);
    Transition(Output(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    Transition(texture.texture.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void DxrCommandContextsApp::RecordBuild(ID3D12GraphicsCommandList4*  list,
                                        const AccelerationStructure& as,
                                        D3D12_GPU_VIRTUAL_ADDRESS    instance)
{
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc = {};
    desc.Inputs                                             = DxrScene::TlasInputs(instance);
    desc.DestAccelerationStructureData                      = as.result->GetGPUVirtualAddress();
    desc.ScratchAccelerationStructureData                   = as.scratch->GetGPUVirtualAddress();
    list->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
}

void DxrCommandContextsApp::BufferBarrier(ID3D12Resource*      buffer,
                                          D3D12_BARRIER_SYNC   sync_before,
                                          D3D12_BARRIER_SYNC   sync_after,
                                          D3D12_BARRIER_ACCESS access_before,
                                          D3D12_BARRIER_ACCESS access_after)
{
    D3D12_BUFFER_BARRIER barrier = { sync_before, sync_after, access_before, access_after, buffer, 0, UINT64_MAX };
    D3D12_BARRIER_GROUP  group   = {};
    group.Type                   = D3D12_BARRIER_TYPE_BUFFER;
    group.NumBarriers            = 1;
    group.pBufferBarriers        = &barrier;
    list7_->Barrier(1, &group);
}

void DxrCommandContextsApp::GlobalBarrier(D3D12_BARRIER_SYNC   sync_before,
                                          D3D12_BARRIER_SYNC   sync_after,
                                          D3D12_BARRIER_ACCESS access_before,
                                          D3D12_BARRIER_ACCESS access_after)
{
    D3D12_GLOBAL_BARRIER barrier = { sync_before, sync_after, access_before, access_after };
    D3D12_BARRIER_GROUP  group   = {};
    group.Type                   = D3D12_BARRIER_TYPE_GLOBAL;
    group.NumBarriers            = 1;
    group.pGlobalBarriers        = &barrier;
    list7_->Barrier(1, &group);
}

D3D12_DISPATCH_RAYS_DESC DxrCommandContextsApp::DescOver(D3D12_GPU_VIRTUAL_ADDRESS base) const
{
    D3D12_DISPATCH_RAYS_DESC desc               = scene_.DispatchDesc(kTileSize, kTileSize);
    desc.RayGenerationShaderRecord.StartAddress = base;
    desc.MissShaderTable.StartAddress           = base + kMissOffset;
    desc.HitGroupTable.StartAddress             = base + kHitOffset;
    desc.HitGroupTable.SizeInBytes              = kRecordStride;
    return desc;
}

void DxrCommandContextsApp::CreateComputeQueue()
{
    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type                     = D3D12_COMMAND_LIST_TYPE_COMPUTE;
    Check(Device()->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&compute_queue_)), "CreateCommandQueue (compute)");
    for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
    {
        Check(
            Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&compute_allocators_[slot])),
            "CreateCommandAllocator (compute)");
        compute_texture_[slot] = CreateTileTexture(L"compute-queue tile");
    }
    Check(Device()->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_COMPUTE, compute_allocators_[0].Get(), nullptr, IID_PPV_ARGS(&compute_list_)),
          "CreateCommandList (compute)");
    Check(compute_list_->Close(), "Close (compute)");
    Check(Device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&compute_fence_)), "CreateFence (compute)");
}

void DxrCommandContextsApp::CreateCopyQueue()
{
    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type                     = D3D12_COMMAND_LIST_TYPE_COPY;
    Check(Device()->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&copy_queue_)), "CreateCommandQueue (copy)");
    for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
    {
        Check(Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&copy_allocators_[slot])),
              "CreateCommandAllocator (copy)");
        copy_instance_upload_[slot]  = CreateMappedBuffer(kDescBytes, L"copy-queue instance upload");
        copy_instance_default_[slot] = CreateBuffer(kDescBytes,
                                                    D3D12_HEAP_TYPE_DEFAULT,
                                                    D3D12_RESOURCE_STATE_COMMON,
                                                    D3D12_RESOURCE_FLAG_NONE,
                                                    L"copy-queue instance");
    }
    Check(Device()->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_COPY, copy_allocators_[0].Get(), nullptr, IID_PPV_ARGS(&copy_list_)),
          "CreateCommandList (copy)");
    Check(copy_list_->Close(), "Close (copy)");
    Check(Device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&copy_fence_)), "CreateFence (copy)");
    copy_tlas_ = CreateAccelerationStructure(DxrScene::TlasInputs(copy_instance_default_[0]->GetGPUVirtualAddress()),
                                             L"TLAS copy-queue");
}

void DxrCommandContextsApp::CreateEnhancedBarriers()
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS12 options12 = {};
    if (FAILED(Device()->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS12, &options12, sizeof(options12))) ||
        !options12.EnhancedBarriersSupported)
    {
        Check(E_FAIL, "enhanced barriers (D3D12_OPTIONS12.EnhancedBarriersSupported)");
    }
    Check(List()->QueryInterface(IID_PPV_ARGS(&list7_)), "QueryInterface ID3D12GraphicsCommandList7");

    // The scene's tables rebuilt in an upload buffer, copied into place every frame.
    ComPtr<ID3D12StateObjectProperties> properties;
    Check(scene_.state_object.As(&properties), "QueryInterface ID3D12StateObjectProperties");
    const void* raygen_id = properties->GetShaderIdentifier(L"RayGen");
    const void* miss_id   = properties->GetShaderIdentifier(L"Miss");
    const void* hit_id    = properties->GetShaderIdentifier(L"HitGroup");
    if ((raygen_id == nullptr) || (miss_id == nullptr) || (hit_id == nullptr))
    {
        Check(E_FAIL, "GetShaderIdentifier");
    }
    HitArguments arguments = {};
    arguments.constant     = 0xA000 | kInstanceIdA;
    arguments.cbv          = scene_.local_cbv[0]->GetGPUVirtualAddress();
    arguments.table        = scene_.local_table_srv[0].gpu;
    std::vector<uint8_t> bytes(kTablesBytes, 0);
    std::memcpy(bytes.data(), raygen_id, kIdentifierBytes);
    std::memcpy(bytes.data() + kMissOffset, miss_id, kIdentifierBytes);
    std::memcpy(bytes.data() + kHitOffset, hit_id, kIdentifierBytes);
    std::memcpy(bytes.data() + kHitOffset + kIdentifierBytes, &arguments, sizeof(arguments));
    barrier_tables_upload_ = CreateUploadBuffer(bytes.data(), bytes.size(), L"enhanced-barriers tables upload");
    barrier_tables_        = CreateBuffer(kTablesBytes,
                                   D3D12_HEAP_TYPE_DEFAULT,
                                   D3D12_RESOURCE_STATE_COMMON,
                                   D3D12_RESOURCE_FLAG_NONE,
                                   L"enhanced-barriers tables");
    for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
    {
        barrier_instance_[slot] = CreateMappedBuffer(kDescBytes, L"enhanced-barriers instance");
    }
    barrier_tlas_ = CreateAccelerationStructure(
        DxrScene::TlasInputs(barrier_instance_[0].buffer->GetGPUVirtualAddress()), L"TLAS enhanced-barriers");
}

void DxrCommandContextsApp::CreateCrossListBuild()
{
    for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
    {
        Check(Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cross_allocators_[slot])),
              "CreateCommandAllocator (cross-list)");
        cross_instance_[slot] = CreateMappedBuffer(kDescBytes, L"cross-list instance");
    }
    Check(Device()->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_DIRECT, cross_allocators_[0].Get(), nullptr, IID_PPV_ARGS(&cross_list_)),
          "CreateCommandList (cross-list)");
    Check(cross_list_->Close(), "Close (cross-list)");
    cross_tlas_ = CreateAccelerationStructure(DxrScene::TlasInputs(cross_instance_[0].buffer->GetGPUVirtualAddress()),
                                              L"TLAS cross-list");
}

void DxrCommandContextsApp::CreateReusedList()
{
    // Recorded once over the scene's static TLAS into a texture of its own; executed every frame as it is.
    reused_texture_ = CreateTileTexture(L"reused-list tile");
    Check(Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&reused_allocator_)),
          "CreateCommandAllocator (reused)");
    Check(Device()->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_DIRECT, reused_allocator_.Get(), nullptr, IID_PPV_ARGS(&reused_list_)),
          "CreateCommandList (reused)");
    ID3D12DescriptorHeap* heaps[] = { DescriptorHeap() };
    reused_list_->SetDescriptorHeaps(1, heaps);
    BindAndDispatch(reused_list_.Get(),
                    0,
                    0,
                    reused_texture_.uav.gpu,
                    scene_.tlas.result->GetGPUVirtualAddress(),
                    scene_.DispatchDesc(kTileSize, kTileSize));
    Check(reused_list_->Close(), "Close (reused)");
}

#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
void DxrCommandContextsApp::CreateBundles()
{
    Check(Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&bundle_allocator_)),
          "CreateCommandAllocator (bundle)");
    ID3D12DescriptorHeap*    heaps[] = { DescriptorHeap() };
    D3D12_DISPATCH_RAYS_DESC desc    = scene_.DispatchDesc(kTileSize, kTileSize);

    // Tile 0: the dispatch itself in a bundle.
    Check(Device()->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_BUNDLE, bundle_allocator_.Get(), nullptr, IID_PPV_ARGS(&bundle_dispatch_)),
          "CreateCommandList (bundle dispatch)");
    bundle_dispatch_->SetDescriptorHeaps(1, heaps);
    BindAndDispatch(bundle_dispatch_.Get(),
                    TileX(kTileBundleDispatch),
                    TileY(kTileBundleDispatch),
                    OutputUav().gpu,
                    scene_.tlas.result->GetGPUVirtualAddress(),
                    desc);
    Check(bundle_dispatch_->Close(), "Close (bundle dispatch)");

    // Tile 1: an indirect dispatch in a bundle, the desc CPU-written into an upload buffer.
    D3D12_INDIRECT_ARGUMENT_DESC argument  = {};
    argument.Type                          = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_RAYS;
    D3D12_COMMAND_SIGNATURE_DESC signature = {};
    signature.ByteStride                   = sizeof(D3D12_DISPATCH_RAYS_DESC);
    signature.NumArgumentDescs             = 1;
    signature.pArgumentDescs               = &argument;
    Check(Device()->CreateCommandSignature(&signature, nullptr, IID_PPV_ARGS(&dispatch_signature_)),
          "CreateCommandSignature");
    indirect_args_ = CreateUploadBuffer(&desc, sizeof(desc), L"bundle-indirect args");
    Check(Device()->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_BUNDLE, bundle_allocator_.Get(), nullptr, IID_PPV_ARGS(&bundle_indirect_)),
          "CreateCommandList (bundle indirect)");
    bundle_indirect_->SetDescriptorHeaps(1, heaps);
    TileConstants constants = { TileX(kTileBundleIndirect), TileY(kTileBundleIndirect), kTileSize, kInstanceIdA };
    bundle_indirect_->SetComputeRootSignature(scene_.global_root_signature.Get());
    bundle_indirect_->SetComputeRoot32BitConstants(0, kTileConstantCount, &constants, 0);
    bundle_indirect_->SetComputeRootDescriptorTable(1, OutputUav().gpu);
    bundle_indirect_->SetComputeRootShaderResourceView(2, scene_.tlas.result->GetGPUVirtualAddress());
    bundle_indirect_->SetPipelineState1(scene_.state_object.Get());
    bundle_indirect_->ExecuteIndirect(dispatch_signature_.Get(), 1, indirect_args_.Get(), 0, nullptr, 0);
    Check(bundle_indirect_->Close(), "Close (bundle indirect)");
}
#endif

void DxrCommandContextsApp::Initialize()
{
    scene_ = CreateDxrScene();
    CreateComputeQueue();
    CreateCopyQueue();
    CreateEnhancedBarriers();
    CreateCrossListBuild();
    CreateReusedList();
#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
    CreateBundles();
#endif
}

void DxrCommandContextsApp::RecordFrame(uint32_t frame, uint32_t slot)
{
    GFXRECON_UNREFERENCED_PARAMETER(frame);
    ID3D12GraphicsCommandList4*    list     = List();
    D3D12_RAYTRACING_INSTANCE_DESC instance = scene_.InstanceDesc(kInstanceIdA, 0);
    D3D12_DISPATCH_RAYS_DESC       desc     = scene_.DispatchDesc(kTileSize, kTileSize);
    D3D12_GPU_VIRTUAL_ADDRESS      tlas_a   = scene_.tlas.result->GetGPUVirtualAddress();

    // The rebuilds below wait for the previous frame's rays over the same structures and their scratch buffers.
    UavBarrier();

    if (TileEnabled(kTileComputeQueue))
    {
        // This slot's texture was last copied two frames ago, which the frame loop waited for.
        const TileTexture& texture = compute_texture_[slot];
        Check(compute_allocators_[slot]->Reset(), "CommandAllocator::Reset (compute)");
        Check(compute_list_->Reset(compute_allocators_[slot].Get(), nullptr), "CommandList::Reset (compute)");
        ID3D12DescriptorHeap* heaps[] = { DescriptorHeap() };
        compute_list_->SetDescriptorHeaps(1, heaps);
        BindAndDispatch(compute_list_.Get(), 0, 0, texture.uav.gpu, tlas_a, desc);
        Check(compute_list_->Close(), "Close (compute)");
        ID3D12CommandList* lists[] = { compute_list_.Get() };
        compute_queue_->ExecuteCommandLists(1, lists);
        Check(compute_queue_->Signal(compute_fence_.Get(), ++compute_fence_value_), "Signal (compute)");
        WaitBefore(compute_fence_.Get(), compute_fence_value_);
        CopyTile(kTileComputeQueue, texture);
    }

    if (TileEnabled(kTileCopyQueue))
    {
        // The CPU writes the desc, the copy queue moves it into the default-heap buffer, the build reads it there.
        std::memcpy(copy_instance_upload_[slot].mapped, &instance, sizeof(instance));
        Check(copy_allocators_[slot]->Reset(), "CommandAllocator::Reset (copy)");
        Check(copy_list_->Reset(copy_allocators_[slot].Get(), nullptr), "CommandList::Reset (copy)");
        copy_list_->CopyBufferRegion(
            copy_instance_default_[slot].Get(), 0, copy_instance_upload_[slot].buffer.Get(), 0, kDescBytes);
        Check(copy_list_->Close(), "Close (copy)");
        ID3D12CommandList* lists[] = { copy_list_.Get() };
        copy_queue_->ExecuteCommandLists(1, lists);
        Check(copy_queue_->Signal(copy_fence_.Get(), ++copy_fence_value_), "Signal (copy)");
        WaitBefore(copy_fence_.Get(), copy_fence_value_);
        BuildAccelerationStructure(copy_tlas_,
                                   DxrScene::TlasInputs(copy_instance_default_[slot]->GetGPUVirtualAddress()));
        BindAndDispatch(list,
                        TileX(kTileCopyQueue),
                        TileY(kTileCopyQueue),
                        OutputUav().gpu,
                        copy_tlas_.result->GetGPUVirtualAddress(),
                        desc);
    }

    if (TileEnabled(kTileEnhancedBarriers))
    {
        // The tables copied into place and the TLAS rebuilt behind enhanced barriers only. The first barrier names
        // the previous frame's read; before any access the debug layer takes the barrier as the first reference.
        BufferBarrier(barrier_tables_.Get(),
                      D3D12_BARRIER_SYNC_RAYTRACING,
                      D3D12_BARRIER_SYNC_COPY,
                      D3D12_BARRIER_ACCESS_SHADER_RESOURCE,
                      D3D12_BARRIER_ACCESS_COPY_DEST);
        list->CopyBufferRegion(barrier_tables_.Get(), 0, barrier_tables_upload_.Get(), 0, kTablesBytes);
        BufferBarrier(barrier_tables_.Get(),
                      D3D12_BARRIER_SYNC_COPY,
                      D3D12_BARRIER_SYNC_RAYTRACING,
                      D3D12_BARRIER_ACCESS_COPY_DEST,
                      D3D12_BARRIER_ACCESS_SHADER_RESOURCE);
        std::memcpy(barrier_instance_[slot].mapped, &instance, sizeof(instance));
        GlobalBarrier(D3D12_BARRIER_SYNC_RAYTRACING,
                      D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE,
                      D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ,
                      D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE);
        RecordBuild(list, barrier_tlas_, barrier_instance_[slot].buffer->GetGPUVirtualAddress());
        GlobalBarrier(D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE,
                      D3D12_BARRIER_SYNC_RAYTRACING,
                      D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE,
                      D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ);
        BindAndDispatch(list,
                        TileX(kTileEnhancedBarriers),
                        TileY(kTileEnhancedBarriers),
                        OutputUav().gpu,
                        barrier_tlas_.result->GetGPUVirtualAddress(),
                        DescOver(barrier_tables_->GetGPUVirtualAddress()));
    }

    if (TileEnabled(kTileCrossListBuild))
    {
        // List A builds; the frame's list dispatches; one ExecuteCommandLists carries both.
        std::memcpy(cross_instance_[slot].mapped, &instance, sizeof(instance));
        Check(cross_allocators_[slot]->Reset(), "CommandAllocator::Reset (cross-list)");
        Check(cross_list_->Reset(cross_allocators_[slot].Get(), nullptr), "CommandList::Reset (cross-list)");
        D3D12_RESOURCE_BARRIER uav = {};
        uav.Type                   = D3D12_RESOURCE_BARRIER_TYPE_UAV;
        cross_list_->ResourceBarrier(1, &uav);
        RecordBuild(cross_list_.Get(), cross_tlas_, cross_instance_[slot].buffer->GetGPUVirtualAddress());
        uav.UAV.pResource = cross_tlas_.result.Get();
        cross_list_->ResourceBarrier(1, &uav);
        Check(cross_list_->Close(), "Close (cross-list)");
        SubmitBefore(cross_list_.Get());
        BindAndDispatch(list,
                        TileX(kTileCrossListBuild),
                        TileY(kTileCrossListBuild),
                        OutputUav().gpu,
                        cross_tlas_.result->GetGPUVirtualAddress(),
                        desc);
    }

    if (TileEnabled(kTileReusedList))
    {
        SubmitBefore(reused_list_.Get());
        CopyTile(kTileReusedList, reused_texture_);
    }

#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
    if (TileEnabled(kTileBundleDispatch))
    {
        list->ExecuteBundle(bundle_dispatch_.Get());
    }
    if (TileEnabled(kTileBundleIndirect))
    {
        list->ExecuteBundle(bundle_indirect_.Get());
    }
#endif
}

GFXRECON_D3D12_TEST_APP_MAIN(DxrCommandContextsApp)
