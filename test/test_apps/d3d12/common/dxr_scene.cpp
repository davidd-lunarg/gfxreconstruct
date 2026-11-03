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

#include "dxr_scene.h"

#include "d3dx12/d3dx12.h"

#include "g_dxr_scene_lib.h"

#include <cstring>
#include <vector>

namespace gfxrecon
{
namespace testapp
{

namespace
{

struct Vertex
{
    float x, y, z;
};

// The hit group record after its identifier: root constants are packed, root descriptors and tables 8-byte aligned.
struct HitArguments
{
    uint32_t                    constant;
    uint32_t                    pad;
    D3D12_GPU_VIRTUAL_ADDRESS   cbv;
    D3D12_GPU_DESCRIPTOR_HANDLE table;
};
static_assert(sizeof(HitArguments) == 24, "hit record layout");

constexpr uint32_t kIdentifierBytes    = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
constexpr uint32_t kHitRecordStride    = 64; // 32 + 24, rounded to 32
constexpr uint32_t kRaygenOffset       = 0;
constexpr uint32_t kMissOffset         = 64;
constexpr uint32_t kHitOffset          = 128;
constexpr uint32_t kTablesBytes        = kHitOffset + 2 * kHitRecordStride;
const wchar_t      kRayGenExport[]     = L"RayGen";
const wchar_t      kClosestHitExport[] = L"ClosestHit";
const wchar_t      kMissExport[]       = L"Miss";
const wchar_t      kHitGroupExport[]   = L"HitGroup";

} // namespace

D3D12_SHADER_BYTECODE DxrSceneLibrary()
{
    return { g_dxr_scene_lib, sizeof(g_dxr_scene_lib) };
}

D3D12_RAYTRACING_INSTANCE_DESC DxrScene::InstanceDesc(uint32_t id, uint32_t contribution) const
{
    D3D12_RAYTRACING_INSTANCE_DESC desc      = {};
    desc.Transform[0][0]                     = 1.0f;
    desc.Transform[1][1]                     = 1.0f;
    desc.Transform[2][2]                     = 1.0f;
    desc.InstanceID                          = id;
    desc.InstanceMask                        = 0xFF;
    desc.InstanceContributionToHitGroupIndex = contribution;
    desc.AccelerationStructure               = blas.result->GetGPUVirtualAddress();
    return desc;
}

D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS DxrScene::TlasInputs(D3D12_GPU_VIRTUAL_ADDRESS instance_descs)
{
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
    inputs.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
    inputs.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    inputs.NumDescs      = 1;
    inputs.DescsLayout   = D3D12_ELEMENTS_LAYOUT_ARRAY;
    inputs.InstanceDescs = instance_descs;
    return inputs;
}

D3D12_DISPATCH_RAYS_DESC DxrScene::DispatchDesc(uint32_t width, uint32_t height) const
{
    D3D12_GPU_VIRTUAL_ADDRESS base              = tables->GetGPUVirtualAddress();
    D3D12_DISPATCH_RAYS_DESC  desc              = {};
    desc.RayGenerationShaderRecord.StartAddress = base + kRaygenOffset;
    desc.RayGenerationShaderRecord.SizeInBytes  = kIdentifierBytes;
    desc.MissShaderTable.StartAddress           = base + kMissOffset;
    desc.MissShaderTable.SizeInBytes            = kIdentifierBytes;
    desc.MissShaderTable.StrideInBytes          = kIdentifierBytes;
    desc.HitGroupTable.StartAddress             = base + kHitOffset;
    desc.HitGroupTable.SizeInBytes              = 2 * kHitRecordStride;
    desc.HitGroupTable.StrideInBytes            = kHitRecordStride;
    desc.Width                                  = width;
    desc.Height                                 = height;
    desc.Depth                                  = 1;
    return desc;
}

DxrScene D3d12TestApp::CreateDxrScene()
{
    DxrScene scene;

    // A quad at z = 1 under the tile's ray origins, which cover [-1, 1].
    const Vertex vertices[4] = {
        { -1.2f, -1.2f, 1.0f }, { 1.2f, -1.2f, 1.0f }, { 1.2f, 1.2f, 1.0f }, { -1.2f, 1.2f, 1.0f }
    };
    const uint16_t indices[6] = { 0, 1, 2, 0, 2, 3 };
    scene.vertices            = CreateUploadBuffer(vertices, sizeof(vertices), L"scene vertices");
    scene.indices             = CreateUploadBuffer(indices, sizeof(indices), L"scene indices");

    scene.geometry.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
    scene.geometry.Flags                                = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
    scene.geometry.Triangles.IndexBuffer                = scene.indices->GetGPUVirtualAddress();
    scene.geometry.Triangles.IndexCount                 = 6;
    scene.geometry.Triangles.IndexFormat                = DXGI_FORMAT_R16_UINT;
    scene.geometry.Triangles.VertexFormat               = DXGI_FORMAT_R32G32B32_FLOAT;
    scene.geometry.Triangles.VertexCount                = 4;
    scene.geometry.Triangles.VertexBuffer.StartAddress  = scene.vertices->GetGPUVirtualAddress();
    scene.geometry.Triangles.VertexBuffer.StrideInBytes = sizeof(Vertex);

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS blas_inputs = {};
    blas_inputs.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
    blas_inputs.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    blas_inputs.NumDescs       = 1;
    blas_inputs.DescsLayout    = D3D12_ELEMENTS_LAYOUT_ARRAY;
    blas_inputs.pGeometryDescs = &scene.geometry;
    scene.blas                 = CreateAccelerationStructure(blas_inputs, L"scene BLAS");
    BuildAccelerationStructure(scene.blas, blas_inputs);

    // The TLAS: built now, from an upload-heap desc that never changes.
    D3D12_RAYTRACING_INSTANCE_DESC instance = scene.InstanceDesc(kInstanceIdA, 0);
    scene.instance                          = CreateUploadBuffer(&instance, sizeof(instance), L"scene instance desc");
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS tlas_inputs =
        DxrScene::TlasInputs(scene.instance->GetGPUVirtualAddress());
    scene.tlas = CreateAccelerationStructure(tlas_inputs, L"scene TLAS");
    BuildAccelerationStructure(scene.tlas, tlas_inputs);

    // Per instance: a 256-byte buffer behind the record's root CBV and a 16-byte buffer behind its descriptor
    // table, each holding the value the closest hit shader expects for that instance.
    for (uint32_t i = 0; i < 2; ++i)
    {
        uint32_t id              = (i == 0) ? kInstanceIdA : kInstanceIdB;
        uint32_t cbv_data[64]    = { 0xB000 | id };
        uint32_t table_data[4]   = { 0xC000 | id };
        scene.local_cbv[i]       = CreateUploadBuffer(cbv_data, sizeof(cbv_data), L"scene local CBV");
        scene.local_table[i]     = CreateUploadBuffer(table_data, sizeof(table_data), L"scene local table");
        scene.local_table_srv[i] = AllocateDescriptor();

        D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
        srv.Format                          = DXGI_FORMAT_R32_TYPELESS;
        srv.ViewDimension                   = D3D12_SRV_DIMENSION_BUFFER;
        srv.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srv.Buffer.NumElements              = 4;
        srv.Buffer.Flags                    = D3D12_BUFFER_SRV_FLAG_RAW;
        Device()->CreateShaderResourceView(scene.local_table[i].Get(), &srv, scene.local_table_srv[i].cpu);
    }

    // Global: the tile constants, the output UAV, the TLAS as a root SRV (DXR) or as a descriptor (inline).
    D3D12_DESCRIPTOR_RANGE uav_range = {
        D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    };
    D3D12_DESCRIPTOR_RANGE srv_range = {
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    };
    D3D12_ROOT_PARAMETER global_params[4] = {};
    global_params[0].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    global_params[0].Constants            = { 0, 0, kTileConstantCount };
    global_params[1].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    global_params[1].DescriptorTable      = { 1, &uav_range };
    global_params[2].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_SRV;
    global_params[2].Descriptor           = { 0, 0 };
    global_params[3].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    global_params[3].DescriptorTable      = { 1, &srv_range };
    D3D12_ROOT_SIGNATURE_DESC global_desc = {};
    global_desc.NumParameters             = 4;
    global_desc.pParameters               = global_params;
    scene.global_root_signature           = CreateRootSignature(global_desc);

    // Local (space1), associated with the hit group: a root constant, a root CBV, a descriptor table.
    D3D12_DESCRIPTOR_RANGE local_range = {
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 1, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    };
    D3D12_ROOT_PARAMETER local_params[3] = {};
    local_params[0].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    local_params[0].Constants            = { 0, 1, 1 };
    local_params[1].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_CBV;
    local_params[1].Descriptor           = { 1, 1 };
    local_params[2].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    local_params[2].DescriptorTable      = { 1, &local_range };
    D3D12_ROOT_SIGNATURE_DESC local_desc = {};
    local_desc.NumParameters             = 3;
    local_desc.pParameters               = local_params;
    local_desc.Flags                     = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
    scene.local_root_signature           = CreateRootSignature(local_desc);

    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    auto                      library = pipeline.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
    D3D12_SHADER_BYTECODE     code    = DxrSceneLibrary();
    library->SetDXILLibrary(&code);
    library->DefineExport(kRayGenExport);
    library->DefineExport(kClosestHitExport);
    library->DefineExport(kMissExport);
    auto hit_group = pipeline.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
    hit_group->SetClosestHitShaderImport(kClosestHitExport);
    hit_group->SetHitGroupExport(kHitGroupExport);
    hit_group->SetHitGroupType(D3D12_HIT_GROUP_TYPE_TRIANGLES);
    auto shader_config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
    shader_config->Config(sizeof(uint32_t), 2 * sizeof(float));
    auto local = pipeline.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
    local->SetRootSignature(scene.local_root_signature.Get());
    auto association = pipeline.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
    association->SetSubobjectToAssociate(*local);
    association->AddExport(kHitGroupExport);
    auto global = pipeline.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
    global->SetRootSignature(scene.global_root_signature.Get());
    auto pipeline_config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
    pipeline_config->Config(1);
    Check(Device()->CreateStateObject(pipeline, IID_PPV_ARGS(&scene.state_object)), "CreateStateObject (scene)");

    // One default-heap buffer: the raygen record at 0, the miss table at 64, the hit group table at 128.
    ComPtr<ID3D12StateObjectProperties> properties;
    Check(scene.state_object.As(&properties), "QueryInterface ID3D12StateObjectProperties");
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
    ShaderTable hit(kHitRecordStride);
    for (uint32_t i = 0; i < 2; ++i)
    {
        HitArguments arguments = {};
        arguments.constant     = 0xA000 | ((i == 0) ? kInstanceIdA : kInstanceIdB);
        arguments.cbv          = scene.local_cbv[i]->GetGPUVirtualAddress();
        arguments.table        = scene.local_table_srv[i].gpu;
        hit.AddRecord(hit_id, &arguments, sizeof(arguments));
    }
    std::vector<uint8_t> bytes(kTablesBytes, 0);
    std::memcpy(bytes.data() + kRaygenOffset, raygen.Bytes().data(), raygen.Size());
    std::memcpy(bytes.data() + kMissOffset, miss.Bytes().data(), miss.Size());
    std::memcpy(bytes.data() + kHitOffset, hit.Bytes().data(), hit.Size());
    scene.tables = CreateDefaultBuffer(bytes.data(),
                                       bytes.size(),
                                       D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
                                       D3D12_RESOURCE_FLAG_NONE,
                                       L"scene shader tables");
    return scene;
}

} // namespace testapp
} // namespace gfxrecon
