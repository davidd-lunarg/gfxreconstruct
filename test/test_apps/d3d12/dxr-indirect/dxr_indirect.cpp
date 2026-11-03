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

// dxr-indirect: ExecuteIndirect on the shared scene (dxr_scene.h). Tiles:
//   0 indirect-cpu        signature {DISPATCH_RAYS}, one command the CPU wrote into an upload buffer
//   1 indirect-gpu-desc   the same command composed by a compute shader each frame from root constants carrying
//                         the table addresses; root constants are not translated at replay, so the desc then holds
//                         capture addresses unless the replay translates values written on the GPU; compiled with
//                         GFXRECON_TESTAPP_GPU_WRITTEN_TILES only
//   2 indirect-count      signature {CONSTANT tile_y, DISPATCH_RAYS}, MaxCommandCount 4, a count buffer holding 3;
//                         command i paints rows i*43.. of the tile, command 3 must stay unexecuted: it is a valid
//                         desc over instance B's hit record, which paints the whole tile red if it runs
//   3 indirect-root-views compute signature {CBV, SRV, UAV, DISPATCH}: a magic value read through each root view
//   4 indirect-ia-views   graphics signature {VBV, IBV, CBV, DRAW_INDEXED}: a quad drawn into the tile
//   5 indirect-cross-list the argument buffer filled and transitioned by a list submitted ahead of this frame's in
//                         the same ExecuteCommandLists, consumed here
// Fail bits: the DXR tiles as dxr_scene.hlsl; tile 3: 1 CBV, 2 SRV, 4 UAV; tile 4: 1 CBV, nothing drawn = black.

#include "dxr_scene.h"

#include "d3dx12/d3dx12.h"

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
#include "g_dxr_indirect_desc_cs.h"
#endif
#include "g_dxr_indirect_quad_ps.h"
#include "g_dxr_indirect_quad_vs.h"
#include "g_dxr_indirect_views_cs.h"

#include <cstring>

using namespace gfxrecon::testapp;

namespace
{

constexpr uint32_t kTileCpu       = 0;
constexpr uint32_t kTileGpuDesc   = 1;
constexpr uint32_t kTileCount     = 2;
constexpr uint32_t kTileRootViews = 3;
constexpr uint32_t kTileIaViews   = 4;
constexpr uint32_t kTileCrossList = 5;
const char* const  kTileNames[]   = { "indirect-cpu",        "indirect-gpu-desc", "indirect-count",
                                      "indirect-root-views", "indirect-ia-views", "indirect-cross-list" };

constexpr uint32_t kMagicCbv       = 0xC0FFEE01;
constexpr uint32_t kMagicSrv       = 0xC0FFEE02;
constexpr uint32_t kMagicUav       = 0xC0FFEE03;
constexpr uint32_t kMagicDraw      = 0xC0FFEE04;
constexpr uint32_t kCountCommands  = 4;
constexpr uint32_t kLiveCommands   = 3;
constexpr uint32_t kRowsPerCommand = 43; // 43 + 43 + 42 rows

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
constexpr bool kGpuWrittenTiles = true;

// The root constants main_desc composes the desc from (dxr_indirect.hlsl).
struct DescParams
{
    uint32_t raygen_start[2];
    uint32_t raygen_size;
    uint32_t pad0;
    uint32_t miss_start[2];
    uint32_t miss_size;
    uint32_t miss_stride;
    uint32_t hit_start[2];
    uint32_t hit_size;
    uint32_t hit_stride;
    uint32_t width;
    uint32_t height;
    uint32_t pad1;
    uint32_t pad2;
};
constexpr UINT kDescParamCount = sizeof(DescParams) / sizeof(uint32_t);
#else
constexpr bool kGpuWrittenTiles = false;
#endif

// Indirect arguments follow their signature with no padding; the structs below match.
#pragma pack(push, 4)
struct CountCommand
{
    uint32_t                 tile_y;
    D3D12_DISPATCH_RAYS_DESC desc;
};
#pragma pack(pop)
static_assert(sizeof(CountCommand) == 4 + sizeof(D3D12_DISPATCH_RAYS_DESC), "count command layout");

struct ViewsCommand
{
    D3D12_GPU_VIRTUAL_ADDRESS cbv;
    D3D12_GPU_VIRTUAL_ADDRESS srv;
    D3D12_GPU_VIRTUAL_ADDRESS uav;
    D3D12_DISPATCH_ARGUMENTS  dispatch;
};
static_assert(sizeof(ViewsCommand) == 40, "views command layout");

struct DrawCommand
{
    D3D12_VERTEX_BUFFER_VIEW     vbv;
    D3D12_INDEX_BUFFER_VIEW      ibv;
    D3D12_GPU_VIRTUAL_ADDRESS    cbv;
    D3D12_DRAW_INDEXED_ARGUMENTS draw;
};
static_assert(sizeof(DrawCommand) == 64, "draw command layout");

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

} // namespace

class DxrIndirectApp : public D3d12TestApp
{
  protected:
    const char*    Name() const override { return "dxr-indirect"; }
    const wchar_t* WindowTitle() const override { return L"GFXR dxr-indirect test"; }
    uint32_t       TileCount() const override { return 6; }
    const char*    TileName(uint32_t i) const override { return kTileNames[i]; }
    bool           TileBuilt(uint32_t i) const override { return kGpuWrittenTiles || (i != kTileGpuDesc); }

    void Initialize() override;
    void RecordFrame(uint32_t frame, uint32_t slot) override;

  private:
    ComPtr<ID3D12CommandSignature> CreateSignature(const D3D12_INDIRECT_ARGUMENT_DESC* args,
                                                   UINT                                count,
                                                   UINT                                stride,
                                                   ID3D12RootSignature*                root_signature);
    // The scene's root signature, output, TLAS, state object and the tile's constants, for an indirect DispatchRays.
    void BindScene(uint32_t tile);

    DxrScene                          scene_;
    ComPtr<ID3D12CommandSignature>    sig_desc_;
    ComPtr<ID3D12CommandSignature>    sig_count_;
    ComPtr<ID3D12CommandSignature>    sig_views_;
    ComPtr<ID3D12CommandSignature>    sig_draw_;
    ComPtr<ID3D12Resource>            args_cpu_;
    ComPtr<ID3D12Resource>            args_gpu_;
    ComPtr<ID3D12Resource>            args_count_;
    ComPtr<ID3D12Resource>            count_buffer_;
    ComPtr<ID3D12Resource>            args_views_;
    ComPtr<ID3D12Resource>            args_draw_;
    ComPtr<ID3D12Resource>            args_cross_;
    ComPtr<ID3D12Resource>            args_cross_source_;
    ComPtr<ID3D12RootSignature>       desc_root_signature_;
    ComPtr<ID3D12RootSignature>       views_root_signature_;
    ComPtr<ID3D12RootSignature>       draw_root_signature_;
    ComPtr<ID3D12PipelineState>       desc_pipeline_;
    ComPtr<ID3D12PipelineState>       views_pipeline_;
    ComPtr<ID3D12PipelineState>       draw_pipeline_;
    ComPtr<ID3D12Resource>            view_cbv_;
    ComPtr<ID3D12Resource>            view_srv_;
    ComPtr<ID3D12Resource>            view_uav_;
    ComPtr<ID3D12Resource>            draw_vertices_;
    ComPtr<ID3D12Resource>            draw_indices_;
    ComPtr<ID3D12Resource>            draw_cbv_;
    ComPtr<ID3D12CommandAllocator>    pre_allocators_[kFramesInFlight];
    ComPtr<ID3D12GraphicsCommandList> pre_list_;
};

ComPtr<ID3D12CommandSignature> DxrIndirectApp::CreateSignature(const D3D12_INDIRECT_ARGUMENT_DESC* args,
                                                               UINT                                count,
                                                               UINT                                stride,
                                                               ID3D12RootSignature*                root_signature)
{
    D3D12_COMMAND_SIGNATURE_DESC desc = {};
    desc.ByteStride                   = stride;
    desc.NumArgumentDescs             = count;
    desc.pArgumentDescs               = args;
    ComPtr<ID3D12CommandSignature> signature;
    Check(Device()->CreateCommandSignature(&desc, root_signature, IID_PPV_ARGS(&signature)), "CreateCommandSignature");
    return signature;
}

void DxrIndirectApp::Initialize()
{
    scene_                        = CreateDxrScene();
    D3D12_DISPATCH_RAYS_DESC desc = scene_.DispatchDesc(kTileSize, kTileSize);

    // Tiles 0, 1 and 5 share the desc-only signature; no root signature, since it sets no root arguments.
    D3D12_INDIRECT_ARGUMENT_DESC dispatch_rays = {};
    dispatch_rays.Type                         = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_RAYS;
    sig_desc_ = CreateSignature(&dispatch_rays, 1, sizeof(D3D12_DISPATCH_RAYS_DESC), nullptr);
    args_cpu_ = CreateUploadBuffer(&desc, sizeof(desc), L"args cpu");

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    // Tile 1: the buffer main_desc writes, and its pipeline.
    args_gpu_                                  = CreateBuffer(256,
                             D3D12_HEAP_TYPE_DEFAULT,
                             D3D12_RESOURCE_STATE_COMMON,
                             D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                             L"args gpu");
    D3D12_ROOT_PARAMETER desc_params[2]        = {};
    desc_params[0].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    desc_params[0].Constants                   = { 0, 0, kDescParamCount };
    desc_params[1].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_UAV;
    desc_params[1].Descriptor                  = { 0, 0 };
    D3D12_ROOT_SIGNATURE_DESC desc_rs          = {};
    desc_rs.NumParameters                      = 2;
    desc_rs.pParameters                        = desc_params;
    desc_root_signature_                       = CreateRootSignature(desc_rs);
    D3D12_COMPUTE_PIPELINE_STATE_DESC desc_pso = {};
    desc_pso.pRootSignature                    = desc_root_signature_.Get();
    desc_pso.CS                                = { g_dxr_indirect_desc_cs, sizeof(g_dxr_indirect_desc_cs) };
    Check(Device()->CreateComputePipelineState(&desc_pso, IID_PPV_ARGS(&desc_pipeline_)),
          "CreateComputePipelineState (desc)");
#endif

    // Tile 2: the constant argument lands in TileConstants::tile_y (dword 1 of root parameter 0).
    D3D12_INDIRECT_ARGUMENT_DESC count_args[2]     = {};
    count_args[0].Type                             = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT;
    count_args[0].Constant.RootParameterIndex      = 0;
    count_args[0].Constant.DestOffsetIn32BitValues = 1;
    count_args[0].Constant.Num32BitValuesToSet     = 1;
    count_args[1].Type                             = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_RAYS;
    sig_count_ = CreateSignature(count_args, 2, sizeof(CountCommand), scene_.global_root_signature.Get());
    CountCommand commands[kCountCommands] = {};
    for (uint32_t i = 0; i < kLiveCommands; ++i)
    {
        commands[i].tile_y      = TileY(kTileCount) + i * kRowsPerCommand;
        commands[i].desc        = desc;
        commands[i].desc.Height = (i + 1 < kLiveCommands) ? kRowsPerCommand : kTileSize - i * kRowsPerCommand;
    }
    // Command 3 is valid, so a driver that reads past the count does not fault, and it shows: every ray hits
    // instance A but finds instance B's record (bits 7).
    CountCommand& beyond = commands[kLiveCommands];
    beyond.tile_y        = TileY(kTileCount);
    beyond.desc          = desc;
    beyond.desc.HitGroupTable.StartAddress += beyond.desc.HitGroupTable.StrideInBytes;
    beyond.desc.HitGroupTable.SizeInBytes = beyond.desc.HitGroupTable.StrideInBytes;
    args_count_                           = CreateUploadBuffer(commands, sizeof(commands), L"args count");
    uint32_t count_value                  = kLiveCommands;
    count_buffer_                         = CreateUploadBuffer(&count_value, sizeof(count_value), L"count buffer");

    // Tile 3: root views delivered by the signature, each over a buffer holding its magic value.
    D3D12_DESCRIPTOR_RANGE uav_range = {
        D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    };
    D3D12_ROOT_PARAMETER views_params[5]        = {};
    views_params[0].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    views_params[0].Constants                   = { 0, 0, kTileConstantCount };
    views_params[1].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    views_params[1].DescriptorTable             = { 1, &uav_range };
    views_params[2].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_CBV;
    views_params[2].Descriptor                  = { 1, 0 };
    views_params[3].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_SRV;
    views_params[3].Descriptor                  = { 0, 0 };
    views_params[4].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_UAV;
    views_params[4].Descriptor                  = { 1, 0 };
    D3D12_ROOT_SIGNATURE_DESC views_rs          = {};
    views_rs.NumParameters                      = 5;
    views_rs.pParameters                        = views_params;
    views_root_signature_                       = CreateRootSignature(views_rs);
    D3D12_COMPUTE_PIPELINE_STATE_DESC views_pso = {};
    views_pso.pRootSignature                    = views_root_signature_.Get();
    views_pso.CS                                = { g_dxr_indirect_views_cs, sizeof(g_dxr_indirect_views_cs) };
    Check(Device()->CreateComputePipelineState(&views_pso, IID_PPV_ARGS(&views_pipeline_)),
          "CreateComputePipelineState (views)");
    uint32_t cbv_data[64]                                = { kMagicCbv };
    uint32_t srv_data[4]                                 = { kMagicSrv };
    uint32_t uav_data[4]                                 = { kMagicUav };
    view_cbv_                                            = CreateUploadBuffer(cbv_data, sizeof(cbv_data), L"view CBV");
    view_srv_                                            = CreateUploadBuffer(srv_data, sizeof(srv_data), L"view SRV");
    view_uav_                                            = CreateDefaultBuffer(uav_data,
                                    sizeof(uav_data),
                                    D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                    D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                    L"view UAV");
    D3D12_INDIRECT_ARGUMENT_DESC views_args[4]           = {};
    views_args[0].Type                                   = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
    views_args[0].ConstantBufferView.RootParameterIndex  = 2;
    views_args[1].Type                                   = D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW;
    views_args[1].ShaderResourceView.RootParameterIndex  = 3;
    views_args[2].Type                                   = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
    views_args[2].UnorderedAccessView.RootParameterIndex = 4;
    views_args[3].Type                                   = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
    sig_views_                 = CreateSignature(views_args, 4, sizeof(ViewsCommand), views_root_signature_.Get());
    ViewsCommand views_command = {};
    views_command.cbv          = view_cbv_->GetGPUVirtualAddress();
    views_command.srv          = view_srv_->GetGPUVirtualAddress();
    views_command.uav          = view_uav_->GetGPUVirtualAddress();
    views_command.dispatch     = { kTileSize / 8, kTileSize / 8, 1 };
    args_views_                = CreateUploadBuffer(&views_command, sizeof(views_command), L"args views");

    // Tile 4: a quad over the tile's viewport, its buffers and root CBV delivered by the signature.
    D3D12_ROOT_PARAMETER draw_params[1] = {};
    draw_params[0].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_CBV;
    draw_params[0].Descriptor           = { 1, 0 };
    D3D12_ROOT_SIGNATURE_DESC draw_rs   = {};
    draw_rs.NumParameters               = 1;
    draw_rs.pParameters                 = draw_params;
    draw_rs.Flags                       = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    draw_root_signature_                = CreateRootSignature(draw_rs);
    D3D12_INPUT_ELEMENT_DESC layout[]   = {
          { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
    D3D12_GRAPHICS_PIPELINE_STATE_DESC draw_pso = {};
    draw_pso.pRootSignature                     = draw_root_signature_.Get();
    draw_pso.VS                                 = { g_dxr_indirect_quad_vs, sizeof(g_dxr_indirect_quad_vs) };
    draw_pso.PS                                 = { g_dxr_indirect_quad_ps, sizeof(g_dxr_indirect_quad_ps) };
    draw_pso.BlendState                         = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    draw_pso.SampleMask                         = UINT_MAX;
    draw_pso.RasterizerState                    = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    draw_pso.RasterizerState.CullMode           = D3D12_CULL_MODE_NONE;
    draw_pso.InputLayout                        = { layout, 1 };
    draw_pso.PrimitiveTopologyType              = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    draw_pso.NumRenderTargets                   = 1;
    draw_pso.RTVFormats[0]                      = DXGI_FORMAT_R8G8B8A8_UNORM;
    draw_pso.SampleDesc.Count                   = 1;
    Check(Device()->CreateGraphicsPipelineState(&draw_pso, IID_PPV_ARGS(&draw_pipeline_)),
          "CreateGraphicsPipelineState (quad)");
    const Vertex quad[4] = {
        { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { -1.0f, 1.0f, 0.0f }
    };
    const uint16_t quad_indices[6]            = { 0, 1, 2, 0, 2, 3 };
    uint32_t       draw_cbv_data[64]          = { kMagicDraw };
    draw_vertices_                            = CreateUploadBuffer(quad, sizeof(quad), L"quad vertices");
    draw_indices_                             = CreateUploadBuffer(quad_indices, sizeof(quad_indices), L"quad indices");
    draw_cbv_                                 = CreateUploadBuffer(draw_cbv_data, sizeof(draw_cbv_data), L"quad CBV");
    D3D12_INDIRECT_ARGUMENT_DESC draw_args[4] = {};
    draw_args[0].Type                         = D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW;
    draw_args[0].VertexBuffer.Slot            = 0;
    draw_args[1].Type                         = D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW;
    draw_args[2].Type                         = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
    draw_args[2].ConstantBufferView.RootParameterIndex = 0;
    draw_args[3].Type                                  = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
    sig_draw_                       = CreateSignature(draw_args, 4, sizeof(DrawCommand), draw_root_signature_.Get());
    DrawCommand draw_command        = {};
    draw_command.vbv.BufferLocation = draw_vertices_->GetGPUVirtualAddress();
    draw_command.vbv.SizeInBytes    = sizeof(quad);
    draw_command.vbv.StrideInBytes  = sizeof(Vertex);
    draw_command.ibv.BufferLocation = draw_indices_->GetGPUVirtualAddress();
    draw_command.ibv.SizeInBytes    = sizeof(quad_indices);
    draw_command.ibv.Format         = DXGI_FORMAT_R16_UINT;
    draw_command.cbv                = draw_cbv_->GetGPUVirtualAddress();
    draw_command.draw               = { 6, 1, 0, 0, 0 };
    args_draw_                      = CreateUploadBuffer(&draw_command, sizeof(draw_command), L"args draw");

    // Tile 5: the argument buffer list A fills each frame from an upload copy of the desc.
    args_cross_source_ = CreateUploadBuffer(&desc, sizeof(desc), L"args cross source");
    args_cross_        = CreateBuffer(
        256, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_FLAG_NONE, L"args cross");
    for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
    {
        Check(Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pre_allocators_[slot])),
              "CreateCommandAllocator (list A)");
    }
    Check(Device()->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_DIRECT, pre_allocators_[0].Get(), nullptr, IID_PPV_ARGS(&pre_list_)),
          "CreateCommandList (list A)");
    Check(pre_list_->Close(), "Close (list A)");
}

void DxrIndirectApp::BindScene(uint32_t tile)
{
    ID3D12GraphicsCommandList4* list      = List();
    TileConstants               constants = { TileX(tile), TileY(tile), kTileSize, kInstanceIdA };
    list->SetComputeRootSignature(scene_.global_root_signature.Get());
    list->SetComputeRoot32BitConstants(0, kTileConstantCount, &constants, 0);
    list->SetComputeRootDescriptorTable(1, OutputUav().gpu);
    list->SetComputeRootShaderResourceView(2, scene_.tlas.result->GetGPUVirtualAddress());
    list->SetPipelineState1(scene_.state_object.Get());
}

void DxrIndirectApp::RecordFrame(uint32_t frame, uint32_t slot)
{
    GFXRECON_UNREFERENCED_PARAMETER(frame);
    ID3D12GraphicsCommandList4* list = List();

    if (TileEnabled(kTileCrossList))
    {
        // List A: the desc copied into the argument buffer and the buffer moved to INDIRECT_ARGUMENT, submitted
        // ahead of this frame's list.
        Check(pre_allocators_[slot]->Reset(), "CommandAllocator::Reset (list A)");
        Check(pre_list_->Reset(pre_allocators_[slot].Get(), nullptr), "CommandList::Reset (list A)");
        pre_list_->CopyBufferRegion(
            args_cross_.Get(), 0, args_cross_source_.Get(), 0, sizeof(D3D12_DISPATCH_RAYS_DESC));
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource   = args_cross_.Get();
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
        pre_list_->ResourceBarrier(1, &barrier);
        Check(pre_list_->Close(), "Close (list A)");
        SubmitBefore(pre_list_.Get());
    }

#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    if (TileEnabled(kTileGpuDesc))
    {
        // The compute shader composes this frame's desc from the table addresses in root constants.
        D3D12_DISPATCH_RAYS_DESC desc   = scene_.DispatchDesc(kTileSize, kTileSize);
        DescParams               params = {};
        SplitAddress(desc.RayGenerationShaderRecord.StartAddress, params.raygen_start);
        params.raygen_size = static_cast<uint32_t>(desc.RayGenerationShaderRecord.SizeInBytes);
        SplitAddress(desc.MissShaderTable.StartAddress, params.miss_start);
        params.miss_size   = static_cast<uint32_t>(desc.MissShaderTable.SizeInBytes);
        params.miss_stride = static_cast<uint32_t>(desc.MissShaderTable.StrideInBytes);
        SplitAddress(desc.HitGroupTable.StartAddress, params.hit_start);
        params.hit_size   = static_cast<uint32_t>(desc.HitGroupTable.SizeInBytes);
        params.hit_stride = static_cast<uint32_t>(desc.HitGroupTable.StrideInBytes);
        params.width      = desc.Width;
        params.height     = desc.Height;
        list->SetComputeRootSignature(desc_root_signature_.Get());
        list->SetPipelineState(desc_pipeline_.Get());
        list->SetComputeRoot32BitConstants(0, kDescParamCount, &params, 0);
        list->SetComputeRootUnorderedAccessView(1, args_gpu_->GetGPUVirtualAddress());
        list->Dispatch(1, 1, 1);
        UavBarrier(args_gpu_.Get());
        Transition(args_gpu_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);
    }
#endif

    if (TileEnabled(kTileCpu))
    {
        BindScene(kTileCpu);
        list->ExecuteIndirect(sig_desc_.Get(), 1, args_cpu_.Get(), 0, nullptr, 0);
    }
#ifdef GFXRECON_TESTAPP_GPU_WRITTEN_TILES
    if (TileEnabled(kTileGpuDesc))
    {
        BindScene(kTileGpuDesc);
        list->ExecuteIndirect(sig_desc_.Get(), 1, args_gpu_.Get(), 0, nullptr, 0);
    }
#endif
    if (TileEnabled(kTileCount))
    {
        BindScene(kTileCount);
        list->ExecuteIndirect(sig_count_.Get(), kCountCommands, args_count_.Get(), 0, count_buffer_.Get(), 0);
    }
    if (TileEnabled(kTileCrossList))
    {
        BindScene(kTileCrossList);
        list->ExecuteIndirect(sig_desc_.Get(), 1, args_cross_.Get(), 0, nullptr, 0);
    }
    if (TileEnabled(kTileRootViews))
    {
        TileConstants constants = { TileX(kTileRootViews), TileY(kTileRootViews), kTileSize, 0 };
        list->SetComputeRootSignature(views_root_signature_.Get());
        list->SetPipelineState(views_pipeline_.Get());
        list->SetComputeRoot32BitConstants(0, kTileConstantCount, &constants, 0);
        list->SetComputeRootDescriptorTable(1, OutputUav().gpu);
        list->ExecuteIndirect(sig_views_.Get(), 1, args_views_.Get(), 0, nullptr, 0);
    }
    if (TileEnabled(kTileIaViews))
    {
        Transition(Output(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RENDER_TARGET);
        D3D12_CPU_DESCRIPTOR_HANDLE rtv = OutputRtv();
        list->OMSetRenderTargets(1, &rtv, FALSE, nullptr);
        D3D12_VIEWPORT viewport = { static_cast<float>(TileX(kTileIaViews)),
                                    static_cast<float>(TileY(kTileIaViews)),
                                    static_cast<float>(kTileSize),
                                    static_cast<float>(kTileSize),
                                    0.0f,
                                    1.0f };
        D3D12_RECT     scissor  = { static_cast<LONG>(TileX(kTileIaViews)),
                                    static_cast<LONG>(TileY(kTileIaViews)),
                                    static_cast<LONG>(TileX(kTileIaViews) + kTileSize),
                                    static_cast<LONG>(TileY(kTileIaViews) + kTileSize) };
        list->RSSetViewports(1, &viewport);
        list->RSSetScissorRects(1, &scissor);
        list->SetGraphicsRootSignature(draw_root_signature_.Get());
        list->SetPipelineState(draw_pipeline_.Get());
        list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        list->ExecuteIndirect(sig_draw_.Get(), 1, args_draw_.Get(), 0, nullptr, 0);
        Transition(Output(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    }
}

GFXRECON_D3D12_TEST_APP_MAIN(DxrIndirectApp)
