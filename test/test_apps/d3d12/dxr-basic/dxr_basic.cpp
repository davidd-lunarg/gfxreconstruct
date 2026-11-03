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

// dxr-basic: the DXR paths every capture has, on the shared scene (dxr_scene.h). TLAS A is the scene's, built once at
// initialization; TLAS B is rebuilt every frame from an instance desc the CPU rewrites. Tiles:
//   0 direct-static   DispatchRays over TLAS A
//   1 direct-rebuilt  DispatchRays over TLAS B
//   2 inline          RayQuery against TLAS A through an SRV descriptor
// Fail bits (dxr_scene.hlsl): 1 root constant, 2 root CBV, 4 descriptor table, 8 InstanceID, 16 miss.

#include "dxr_scene.h"

#include "g_dxr_basic_inline_cs.h"

#include <cstring>

using namespace gfxrecon::testapp;

namespace
{

constexpr uint32_t kTileDirectStatic  = 0;
constexpr uint32_t kTileDirectRebuilt = 1;
constexpr uint32_t kTileInline        = 2;
const char* const  kTileNames[]       = { "direct-static", "direct-rebuilt", "inline" };

} // namespace

class DxrBasicApp : public D3d12TestApp
{
  protected:
    const char*    Name() const override { return "dxr-basic"; }
    const wchar_t* WindowTitle() const override { return L"GFXR dxr-basic test"; }
    uint32_t       TileCount() const override { return 3; }
    const char*    TileName(uint32_t i) const override { return kTileNames[i]; }

    void Initialize() override;
    void RecordFrame(uint32_t frame, uint32_t slot) override;

  private:
    void DispatchTile(uint32_t tile, D3D12_GPU_VIRTUAL_ADDRESS tlas, uint32_t expected_instance);

    DxrScene                    scene_;
    AccelerationStructure       tlas_b_;
    ComPtr<ID3D12Resource>      instance_b_[kFramesInFlight];
    void*                       instance_b_mapped_[kFramesInFlight]{};
    DescriptorHandle            tlas_a_srv_;
    ComPtr<ID3D12PipelineState> inline_pipeline_;
};

void DxrBasicApp::Initialize()
{
    scene_ = CreateDxrScene();

    // TLAS B: the scene's shape, rebuilt every frame from a per-slot desc the CPU rewrites.
    tlas_b_ = CreateAccelerationStructure(DxrScene::TlasInputs(scene_.instance->GetGPUVirtualAddress()), L"TLAS B");
    for (uint32_t slot = 0; slot < kFramesInFlight; ++slot)
    {
        instance_b_[slot]   = CreateUploadBuffer(nullptr, sizeof(D3D12_RAYTRACING_INSTANCE_DESC), L"instance desc B");
        D3D12_RANGE no_read = { 0, 0 };
        Check(instance_b_[slot]->Map(0, &no_read, &instance_b_mapped_[slot]), "Map (instance desc B)");
    }

    // TLAS A through a descriptor, for the inline tile.
    tlas_a_srv_                                       = AllocateDescriptor();
    D3D12_SHADER_RESOURCE_VIEW_DESC tlas_srv          = {};
    tlas_srv.Format                                   = DXGI_FORMAT_UNKNOWN;
    tlas_srv.ViewDimension                            = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
    tlas_srv.Shader4ComponentMapping                  = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    tlas_srv.RaytracingAccelerationStructure.Location = scene_.tlas.result->GetGPUVirtualAddress();
    Device()->CreateShaderResourceView(nullptr, &tlas_srv, tlas_a_srv_.cpu);

    D3D12_COMPUTE_PIPELINE_STATE_DESC compute = {};
    compute.pRootSignature                    = scene_.global_root_signature.Get();
    compute.CS                                = { g_dxr_basic_inline_cs, sizeof(g_dxr_basic_inline_cs) };
    Check(Device()->CreateComputePipelineState(&compute, IID_PPV_ARGS(&inline_pipeline_)),
          "CreateComputePipelineState (inline)");
}

void DxrBasicApp::RecordFrame(uint32_t frame, uint32_t slot)
{
    GFXRECON_UNREFERENCED_PARAMETER(frame);
    ID3D12GraphicsCommandList4* list = List();

    if (TileEnabled(kTileDirectRebuilt))
    {
        // The CPU rewrites this slot's instance desc, then the frame rebuilds TLAS B from it. The barrier orders the
        // rebuild after the previous frame's dispatch over TLAS B and its use of the scratch buffer.
        D3D12_RAYTRACING_INSTANCE_DESC desc = scene_.InstanceDesc(kInstanceIdB, 1);
        std::memcpy(instance_b_mapped_[slot], &desc, sizeof(desc));
        UavBarrier();
        BuildAccelerationStructure(tlas_b_, DxrScene::TlasInputs(instance_b_[slot]->GetGPUVirtualAddress()));
    }

    list->SetComputeRootSignature(scene_.global_root_signature.Get());
    list->SetComputeRootDescriptorTable(1, OutputUav().gpu);
    if (TileEnabled(kTileDirectStatic))
    {
        DispatchTile(kTileDirectStatic, scene_.tlas.result->GetGPUVirtualAddress(), kInstanceIdA);
    }
    if (TileEnabled(kTileDirectRebuilt))
    {
        DispatchTile(kTileDirectRebuilt, tlas_b_.result->GetGPUVirtualAddress(), kInstanceIdB);
    }
    if (TileEnabled(kTileInline))
    {
        TileConstants constants = { TileX(kTileInline), TileY(kTileInline), kTileSize, kInstanceIdA };
        list->SetPipelineState(inline_pipeline_.Get());
        list->SetComputeRoot32BitConstants(0, kTileConstantCount, &constants, 0);
        list->SetComputeRootDescriptorTable(3, tlas_a_srv_.gpu);
        list->Dispatch(kTileSize / 8, kTileSize / 8, 1);
    }
}

void DxrBasicApp::DispatchTile(uint32_t tile, D3D12_GPU_VIRTUAL_ADDRESS tlas, uint32_t expected_instance)
{
    ID3D12GraphicsCommandList4* list      = List();
    TileConstants               constants = { TileX(tile), TileY(tile), kTileSize, expected_instance };
    list->SetComputeRoot32BitConstants(0, kTileConstantCount, &constants, 0);
    list->SetComputeRootShaderResourceView(2, tlas);
    list->SetPipelineState1(scene_.state_object.Get());
    D3D12_DISPATCH_RAYS_DESC desc = scene_.DispatchDesc(kTileSize, kTileSize);
    list->DispatchRays(&desc);
}

GFXRECON_D3D12_TEST_APP_MAIN(DxrBasicApp)
