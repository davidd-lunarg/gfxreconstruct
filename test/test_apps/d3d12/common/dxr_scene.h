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

#ifndef GFXRECON_TESTAPP_DXR_SCENE_H
#define GFXRECON_TESTAPP_DXR_SCENE_H

// The DXR scene the test apps share: one quad at z = 1 in a BLAS, a TLAS over one instance of it (InstanceID
// kInstanceIdA), a state object whose hit group record carries a root constant, a root CBV and a descriptor table,
// and the three shader tables in one default-heap buffer. Every ray of a tile, traced from (x, y, 0) in [-1, 1]^2
// toward +Z, hits the quad; the closest hit shader (dxr_scene.hlsl) checks the record's values against the instance
// it hit and the raygen shader paints the tile. Fail bits: 1 root constant, 2 root CBV, 4 descriptor table,
// 8 InstanceID differs from TileConstants::expected_instance (kAnyInstance accepts all), 16 miss.
//
// Global root signature: 0 = TileConstants (b0), 1 = descriptor table with the output UAV (u0), 2 = root SRV (t0,
// the TLAS), 3 = descriptor table with an acceleration structure SRV (t1, for RayQuery). Hit group table: record 0
// for instance kInstanceIdA (InstanceContributionToHitGroupIndex 0), record 1 for kInstanceIdB (contribution 1).

#include "d3d12_test_app.h"

namespace gfxrecon
{
namespace testapp
{

constexpr uint32_t kInstanceIdA = 0x2A;
constexpr uint32_t kInstanceIdB = 0x3B;
constexpr uint32_t kAnyInstance = 0xFFFFFFFF; // TileConstants::expected_instance accepting every InstanceID

struct TileConstants
{
    uint32_t tile_x;
    uint32_t tile_y;
    uint32_t tile_size;
    uint32_t expected_instance;
};
constexpr UINT kTileConstantCount = sizeof(TileConstants) / sizeof(uint32_t);

struct DxrScene
{
    ComPtr<ID3D12Resource>         vertices;
    ComPtr<ID3D12Resource>         indices;
    D3D12_RAYTRACING_GEOMETRY_DESC geometry{};
    AccelerationStructure          blas;
    ComPtr<ID3D12Resource>         instance; // the upload-heap desc the TLAS was built from
    AccelerationStructure          tlas;
    ComPtr<ID3D12Resource>         local_cbv[2];
    ComPtr<ID3D12Resource>         local_table[2];
    DescriptorHandle               local_table_srv[2];
    ComPtr<ID3D12RootSignature>    global_root_signature;
    ComPtr<ID3D12RootSignature>    local_root_signature;
    ComPtr<ID3D12StateObject>      state_object;
    ComPtr<ID3D12Resource>         tables;

    // An instance of the quad's BLAS with the identity transform.
    D3D12_RAYTRACING_INSTANCE_DESC InstanceDesc(uint32_t id, uint32_t contribution) const;

    // A one-instance top-level build reading the desc at `instance_descs`.
    static D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS TlasInputs(D3D12_GPU_VIRTUAL_ADDRESS instance_descs);

    // The three tables and the dispatch size.
    D3D12_DISPATCH_RAYS_DESC DispatchDesc(uint32_t width, uint32_t height) const;
};

// The compiled dxr_scene.hlsl library.
D3D12_SHADER_BYTECODE DxrSceneLibrary();

} // namespace testapp
} // namespace gfxrecon

#endif // GFXRECON_TESTAPP_DXR_SCENE_H
