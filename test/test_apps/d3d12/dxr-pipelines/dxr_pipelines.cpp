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

// dxr-pipelines: the ways a raytracing state object is assembled, on the shared scene (dxr_scene.h). Every tile is a
// pipeline of its own dispatched over the scene's quad, with hit records whose local root arguments the replay has to
// translate, so a local root signature the replay fails to resolve shows as a red tile or a fault. Tiles:
//   0 collection-hitgroup  the closest hit shader, its local root signature and their association live in a
//                          COLLECTION; the pipeline includes it (EXISTING_COLLECTION) and defines the hit group that
//                          imports the shader, whose root signature is therefore the import's
//   1 collection-two-lrs   two collections exporting the shader under two names with root signatures of different
//                          layouts, two hit groups, two instances: each import brings its own signature
//   2 collection-explicit  the collection leaves its shader unassociated (ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_
//                          DEFINITIONS) and the pipeline associates the root signature to the hit group export itself
//   3 collection-released  tile 0's shape with the collection released once the pipeline exists, before any trim
//   4 add-to-state-object  a pipeline grown with AddToStateObject: the addition brings a shader of its own name, a hit
//                          group, a root signature and the association; the original is released; both hit groups
//                          are dispatched
//   5 dxil-defined-lrs     the local root signature and its association declared in the DXIL library
//                          (dxr_pipelines_dxil_lrs.hlsl), no LOCAL_ROOT_SIGNATURE subobject in the description
//   6 callable             the closest hit calls a callable shader that reads the root CBV of its own local root
//                          signature; the dispatch carries a callable shader table
//   7 implicit-default     one local root signature with no association at all, the default for every export, so the
//                          raygen and miss records carry the arguments too
// Tiles 0, 1, 3 and 5 reproduce known replay defects and are compiled with GFXRECON_TESTAPP_KNOWN_DEFECT_TILES only.
// Fail bits (dxr_scene.hlsl, dxr_pipelines.hlsl): 1 root constant, 2 root CBV (the callable's in tile 6), 4 descriptor
// table, 8 InstanceID, 16 miss.

#include "dxr_scene.h"

#include "d3dx12/d3dx12.h"

#include "g_dxr_pipelines_lib.h"
#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
#include "g_dxr_pipelines_dxil_lrs_lib.h"
#endif

#include <cstring>
#include <cwchar>
#include <initializer_list>
#include <vector>

using namespace gfxrecon::testapp;

namespace
{

constexpr uint32_t kTileCollectionHitGroup = 0;
constexpr uint32_t kTileCollectionTwoLrs   = 1;
constexpr uint32_t kTileCollectionExplicit = 2;
constexpr uint32_t kTileCollectionReleased = 3;
constexpr uint32_t kTileAddToStateObject   = 4;
constexpr uint32_t kTileDxilDefinedLrs     = 5;
constexpr uint32_t kTileCallable           = 6;
constexpr uint32_t kTileImplicitDefault    = 7;
constexpr uint32_t kTileCount              = 8;
const char* const  kTileNames[kTileCount]  = {
      "collection-hitgroup", "collection-two-lrs", "collection-explicit", "collection-released", "add-to-state-object",
      "dxil-defined-lrs",    "callable",           "implicit-default"
};

#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
constexpr bool kDefectTiles = true;
#else
constexpr bool kDefectTiles = false;
#endif

constexpr uint32_t kRecordStride  = 64; // a 32-byte identifier and up to 24 bytes of arguments, rounded to 32
constexpr uint32_t kCallableMagic = 0xD0D0D0D0;

const wchar_t kRayGen[]         = L"RayGen";
const wchar_t kMiss[]           = L"Miss";
const wchar_t kClosestHit[]     = L"ClosestHit";
const wchar_t kHitGroup[]       = L"HitGroup";
const wchar_t kClosestHitA[]    = L"ClosestHitA";
const wchar_t kClosestHitB[]    = L"ClosestHitB";
const wchar_t kHitGroupA[]      = L"HitGroupA";
const wchar_t kHitGroupB[]      = L"HitGroupB";
const wchar_t kClosestHitX[]    = L"ClosestHitX";
const wchar_t kHitGroupX[]      = L"HitGroupX";
const wchar_t kClosestHitR[]    = L"ClosestHitR";
const wchar_t kHitGroupR[]      = L"HitGroupR";
const wchar_t kClosestHitAdd[]  = L"ClosestHitAdd";
const wchar_t kHitGroupAdd[]    = L"HitGroupAdd";
const wchar_t kClosestHitDxil[] = L"ClosestHitDxil";
const wchar_t kHitGroupDxil[]   = L"HitGroupDxil";
const wchar_t kDxilLrs[]        = L"DxilLocalRootSignature";
const wchar_t kDxilLrsAssoc[]   = L"DxilLocalRootSignatureAssociation";
const wchar_t kClosestHitCall[] = L"ClosestHitCall";
const wchar_t kHitGroupCall[]   = L"HitGroupCall";
const wchar_t kCallable[]       = L"Callable";

// Record arguments in local root signature A's order (the scene's, dxr_scene.h): the constant, then the CBV and the
// table at 8-byte alignment.
struct ArgsA
{
    uint32_t                    constant;
    uint32_t                    pad;
    D3D12_GPU_VIRTUAL_ADDRESS   cbv;
    D3D12_GPU_DESCRIPTOR_HANDLE table;
};
// Local root signature B: the same three parameters declared in the order table, CBV, constant.
struct ArgsB
{
    D3D12_GPU_DESCRIPTOR_HANDLE table;
    D3D12_GPU_VIRTUAL_ADDRESS   cbv;
    uint32_t                    constant;
    uint32_t                    pad;
};
static_assert((sizeof(ArgsA) == 24) && (sizeof(ArgsB) == 24), "record arguments");

// An export of a DXIL library: `rename` is the library's own name when it is exported under another, else null.
struct Export
{
    LPCWSTR name;
    LPCWSTR rename;
};

// A tile's pipeline and its dispatch.
struct TileDispatch
{
    ComPtr<ID3D12StateObject> state_object;
    ComPtr<ID3D12Resource>    tables;
    D3D12_DISPATCH_RAYS_DESC  desc{};
    D3D12_GPU_VIRTUAL_ADDRESS tlas{ 0 };
    uint32_t                  expected_instance{ kInstanceIdA };
};

} // namespace

class DxrPipelinesApp : public D3d12TestApp
{
  protected:
    const char*    Name() const override { return "dxr-pipelines"; }
    const wchar_t* WindowTitle() const override { return L"GFXR dxr-pipelines test"; }
    uint32_t       TileCount() const override { return kTileCount; }
    const char*    TileName(uint32_t i) const override { return kTileNames[i]; }
    bool           TileBuilt(uint32_t i) const override
    {
        return kDefectTiles || ((i != kTileCollectionHitGroup) && (i != kTileCollectionTwoLrs) &&
                                (i != kTileCollectionReleased) && (i != kTileDxilDefinedLrs));
    }

    void Initialize() override;
    void RecordFrame(uint32_t frame, uint32_t slot) override;

  private:
    // The subobjects every pipeline and collection here shares: the scene's global root signature, the shader config
    // and the pipeline config.
    void AddCommon(CD3DX12_STATE_OBJECT_DESC& desc);
    void AddLibrary(CD3DX12_STATE_OBJECT_DESC& desc, D3D12_SHADER_BYTECODE code, std::initializer_list<Export> exports);
    void AddHitGroup(CD3DX12_STATE_OBJECT_DESC& desc, LPCWSTR name, LPCWSTR closest_hit);
    // A LOCAL_ROOT_SIGNATURE subobject associated with `exports`; none: no association subobject, so it is the default.
    void AddLocalRootSignature(CD3DX12_STATE_OBJECT_DESC&     desc,
                               ID3D12RootSignature*           root_signature,
                               std::initializer_list<LPCWSTR> exports);
    void AddFlags(CD3DX12_STATE_OBJECT_DESC& desc, D3D12_STATE_OBJECT_FLAGS flags);
    // The helper finalizes its description on conversion, so the reference is not const.
    ComPtr<ID3D12StateObject> Create(CD3DX12_STATE_OBJECT_DESC& desc, const char* what);
    // A collection of the scene's closest hit shader exported as `export_name`, with `root_signature` associated to it;
    // null leaves the shader unassociated and lets the collection depend on the including state object for it. Hit
    // groups of the including state object may import the shader.
    ComPtr<ID3D12StateObject> CreateHitCollection(LPCWSTR export_name, ID3D12RootSignature* root_signature);
    const void*               Identifier(ID3D12StateObject* state_object, LPCWSTR export_name);
    // A one-record table of `export_name`'s identifier followed by `arguments`.
    ShaderTable
    Record(ID3D12StateObject* state_object, LPCWSTR export_name, const void* arguments = nullptr, size_t size = 0);
    ArgsA ArgumentsA(uint32_t instance) const;
    ArgsB ArgumentsB(uint32_t instance) const;
    // The tile's tables in one buffer (the raygen record, the miss table, the hit group table and, when given, the
    // callable table at 64-byte offsets) and its dispatch desc.
    void SetTables(TileDispatch&      tile,
                   const ShaderTable& raygen,
                   const ShaderTable& miss,
                   const ShaderTable& hit,
                   const ShaderTable* callable = nullptr);
    // The pipeline of the tile whose closest hit comes from `collection` under `export_name`, as hit group `hit_group`,
    // with the hit record of instance A in local root signature A's layout.
    void CreateCollectionTile(uint32_t tile, ID3D12StateObject* collection, LPCWSTR export_name, LPCWSTR hit_group);

    void CreateCollectionExplicit();
    void CreateAddToStateObject();
    void CreateCallable();
    void CreateImplicitDefault();
#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
    void CreateCollectionHitGroup();
    void CreateCollectionTwoLrs();
    void CreateCollectionReleased();
    void CreateDxilDefinedLrs();
#endif

    DxrScene                               scene_;
    ComPtr<ID3D12Resource>                 two_instance_descs_;
    AccelerationStructure                  tlas_two_;
    ComPtr<ID3D12RootSignature>            local_root_signature_b_;
    ComPtr<ID3D12RootSignature>            callable_root_signature_;
    ComPtr<ID3D12Resource>                 callable_cbv_;
    std::vector<ComPtr<ID3D12StateObject>> collections_; // kept alive for the pipelines that include them
    TileDispatch                           tiles_[kTileCount];
};

void DxrPipelinesApp::AddCommon(CD3DX12_STATE_OBJECT_DESC& desc)
{
    auto shader_config = desc.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
    shader_config->Config(sizeof(uint32_t), 2 * sizeof(float));
    auto global = desc.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
    global->SetRootSignature(scene_.global_root_signature.Get());
    auto pipeline_config = desc.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
    pipeline_config->Config(1);
}

void DxrPipelinesApp::AddLibrary(CD3DX12_STATE_OBJECT_DESC&    desc,
                                 D3D12_SHADER_BYTECODE         code,
                                 std::initializer_list<Export> exports)
{
    auto library = desc.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
    library->SetDXILLibrary(&code);
    for (const Export& e : exports)
    {
        library->DefineExport(e.name, e.rename);
    }
}

void DxrPipelinesApp::AddHitGroup(CD3DX12_STATE_OBJECT_DESC& desc, LPCWSTR name, LPCWSTR closest_hit)
{
    auto hit_group = desc.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
    hit_group->SetClosestHitShaderImport(closest_hit);
    hit_group->SetHitGroupExport(name);
    hit_group->SetHitGroupType(D3D12_HIT_GROUP_TYPE_TRIANGLES);
}

void DxrPipelinesApp::AddLocalRootSignature(CD3DX12_STATE_OBJECT_DESC&     desc,
                                            ID3D12RootSignature*           root_signature,
                                            std::initializer_list<LPCWSTR> exports)
{
    auto local = desc.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
    local->SetRootSignature(root_signature);
    if (exports.size() > 0)
    {
        auto association = desc.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
        association->SetSubobjectToAssociate(*local);
        for (LPCWSTR e : exports)
        {
            association->AddExport(e);
        }
    }
}

void DxrPipelinesApp::AddFlags(CD3DX12_STATE_OBJECT_DESC& desc, D3D12_STATE_OBJECT_FLAGS flags)
{
    auto config = desc.CreateSubobject<CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT>();
    config->SetFlags(flags);
}

ComPtr<ID3D12StateObject> DxrPipelinesApp::Create(CD3DX12_STATE_OBJECT_DESC& desc, const char* what)
{
    ComPtr<ID3D12StateObject> state_object;
    Check(Device()->CreateStateObject(desc, IID_PPV_ARGS(&state_object)), what);
    return state_object;
}

ComPtr<ID3D12StateObject> DxrPipelinesApp::CreateHitCollection(LPCWSTR export_name, ID3D12RootSignature* root_signature)
{
    CD3DX12_STATE_OBJECT_DESC collection{ D3D12_STATE_OBJECT_TYPE_COLLECTION };
    LPCWSTR                   rename = (wcscmp(export_name, kClosestHit) == 0) ? nullptr : kClosestHit;
    AddLibrary(collection, DxrSceneLibrary(), { { export_name, rename } });
    D3D12_STATE_OBJECT_FLAGS flags = D3D12_STATE_OBJECT_FLAG_ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS;
    if (root_signature != nullptr)
    {
        AddLocalRootSignature(collection, root_signature, { export_name });
    }
    else
    {
        flags |= D3D12_STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS;
    }
    AddFlags(collection, flags);
    AddCommon(collection);
    return Create(collection, "CreateStateObject (collection)");
}

const void* DxrPipelinesApp::Identifier(ID3D12StateObject* state_object, LPCWSTR export_name)
{
    ComPtr<ID3D12StateObjectProperties> properties;
    Check(state_object->QueryInterface(IID_PPV_ARGS(&properties)), "QueryInterface ID3D12StateObjectProperties");
    const void* identifier = properties->GetShaderIdentifier(export_name);
    if (identifier == nullptr)
    {
        printf("%s: no shader identifier for %ls\n", Name(), export_name);
        Check(E_FAIL, "GetShaderIdentifier");
    }
    return identifier;
}

ShaderTable
DxrPipelinesApp::Record(ID3D12StateObject* state_object, LPCWSTR export_name, const void* arguments, size_t size)
{
    ShaderTable table(kRecordStride);
    table.AddRecord(Identifier(state_object, export_name), arguments, size);
    return table;
}

ArgsA DxrPipelinesApp::ArgumentsA(uint32_t instance) const
{
    ArgsA args    = {};
    args.constant = 0xA000 | ((instance == 0) ? kInstanceIdA : kInstanceIdB);
    args.cbv      = scene_.local_cbv[instance]->GetGPUVirtualAddress();
    args.table    = scene_.local_table_srv[instance].gpu;
    return args;
}

ArgsB DxrPipelinesApp::ArgumentsB(uint32_t instance) const
{
    ArgsA a       = ArgumentsA(instance);
    ArgsB args    = {};
    args.table    = a.table;
    args.cbv      = a.cbv;
    args.constant = a.constant;
    return args;
}

void DxrPipelinesApp::SetTables(TileDispatch&      tile,
                                const ShaderTable& raygen,
                                const ShaderTable& miss,
                                const ShaderTable& hit,
                                const ShaderTable* callable)
{
    const uint32_t       miss_offset     = kRecordStride;
    const uint32_t       hit_offset      = 2 * kRecordStride;
    const uint32_t       callable_offset = hit_offset + hit.Size();
    const uint32_t       callable_size   = (callable != nullptr) ? callable->Size() : 0;
    std::vector<uint8_t> bytes(callable_offset + callable_size, 0);
    std::memcpy(bytes.data(), raygen.Bytes().data(), raygen.Size());
    std::memcpy(bytes.data() + miss_offset, miss.Bytes().data(), miss.Size());
    std::memcpy(bytes.data() + hit_offset, hit.Bytes().data(), hit.Size());
    if (callable != nullptr)
    {
        std::memcpy(bytes.data() + callable_offset, callable->Bytes().data(), callable_size);
    }
    tile.tables = CreateDefaultBuffer(bytes.data(),
                                      bytes.size(),
                                      D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
                                      D3D12_RESOURCE_FLAG_NONE,
                                      L"pipeline tables");

    D3D12_GPU_VIRTUAL_ADDRESS base              = tile.tables->GetGPUVirtualAddress();
    D3D12_DISPATCH_RAYS_DESC& desc              = tile.desc;
    desc                                        = {};
    desc.RayGenerationShaderRecord.StartAddress = base;
    desc.RayGenerationShaderRecord.SizeInBytes  = kRecordStride;
    desc.MissShaderTable.StartAddress           = base + miss_offset;
    desc.MissShaderTable.SizeInBytes            = kRecordStride;
    desc.MissShaderTable.StrideInBytes          = kRecordStride;
    desc.HitGroupTable.StartAddress             = base + hit_offset;
    desc.HitGroupTable.SizeInBytes              = hit.Size();
    desc.HitGroupTable.StrideInBytes            = kRecordStride;
    if (callable != nullptr)
    {
        desc.CallableShaderTable.StartAddress  = base + callable_offset;
        desc.CallableShaderTable.SizeInBytes   = callable_size;
        desc.CallableShaderTable.StrideInBytes = kRecordStride;
    }
    desc.Width  = kTileSize;
    desc.Height = kTileSize;
    desc.Depth  = 1;
}

void DxrPipelinesApp::CreateCollectionTile(uint32_t           tile_index,
                                           ID3D12StateObject* collection,
                                           LPCWSTR            export_name,
                                           LPCWSTR            hit_group)
{
    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(pipeline, DxrSceneLibrary(), { { kRayGen, nullptr }, { kMiss, nullptr } });
    auto existing = pipeline.CreateSubobject<CD3DX12_EXISTING_COLLECTION_SUBOBJECT>();
    existing->SetExistingCollection(collection);
    AddHitGroup(pipeline, hit_group, export_name);
    AddCommon(pipeline);

    TileDispatch& tile = tiles_[tile_index];
    tile.state_object  = Create(pipeline, "CreateStateObject (pipeline from a collection)");
    ArgsA args         = ArgumentsA(0);
    SetTables(tile,
              Record(tile.state_object.Get(), kRayGen),
              Record(tile.state_object.Get(), kMiss),
              Record(tile.state_object.Get(), hit_group, &args, sizeof(args)));
    tile.tlas = scene_.tlas.result->GetGPUVirtualAddress();
}

void DxrPipelinesApp::CreateCollectionExplicit()
{
    // The collection's shader has no association; the pipeline gives its hit group export one.
    ComPtr<ID3D12StateObject> collection = CreateHitCollection(kClosestHitX, nullptr);
    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(pipeline, DxrSceneLibrary(), { { kRayGen, nullptr }, { kMiss, nullptr } });
    auto existing = pipeline.CreateSubobject<CD3DX12_EXISTING_COLLECTION_SUBOBJECT>();
    existing->SetExistingCollection(collection.Get());
    AddHitGroup(pipeline, kHitGroupX, kClosestHitX);
    AddLocalRootSignature(pipeline, scene_.local_root_signature.Get(), { kHitGroupX });
    AddCommon(pipeline);

    TileDispatch& tile = tiles_[kTileCollectionExplicit];
    tile.state_object  = Create(pipeline, "CreateStateObject (collection-explicit)");
    ArgsA args         = ArgumentsA(0);
    SetTables(tile,
              Record(tile.state_object.Get(), kRayGen),
              Record(tile.state_object.Get(), kMiss),
              Record(tile.state_object.Get(), kHitGroupX, &args, sizeof(args)));
    tile.tlas = scene_.tlas.result->GetGPUVirtualAddress();
    collections_.push_back(collection);
}

void DxrPipelinesApp::CreateAddToStateObject()
{
    // The original: the scene's shape with hit group A, open to additions.
    CD3DX12_STATE_OBJECT_DESC base{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(base, DxrSceneLibrary(), { { kRayGen, nullptr }, { kMiss, nullptr }, { kClosestHit, nullptr } });
    AddHitGroup(base, kHitGroup, kClosestHit);
    AddLocalRootSignature(base, scene_.local_root_signature.Get(), { kHitGroup });
    AddFlags(base, D3D12_STATE_OBJECT_FLAG_ALLOW_STATE_OBJECT_ADDITIONS);
    AddCommon(base);
    ComPtr<ID3D12StateObject> original = Create(base, "CreateStateObject (add-to-state-object original)");

    // The addition: self-contained (its shader under a name of its own, the hit group, root signature B and the
    // association), with the global subobjects repeated as the spec requires them consistent.
    CD3DX12_STATE_OBJECT_DESC addition{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(addition, DxrSceneLibrary(), { { kClosestHitAdd, kClosestHit } });
    AddHitGroup(addition, kHitGroupAdd, kClosestHitAdd);
    AddLocalRootSignature(addition, local_root_signature_b_.Get(), { kHitGroupAdd });
    AddFlags(addition, D3D12_STATE_OBJECT_FLAG_ALLOW_STATE_OBJECT_ADDITIONS);
    AddCommon(addition);
    ComPtr<ID3D12Device7> device7;
    Check(Device()->QueryInterface(IID_PPV_ARGS(&device7)), "QueryInterface ID3D12Device7");
    TileDispatch& tile = tiles_[kTileAddToStateObject];
    Check(device7->AddToStateObject(addition, original.Get(), IID_PPV_ARGS(&tile.state_object)), "AddToStateObject");

    // The original's identifiers stay valid in the grown object; the original itself is released at the end of this
    // scope, so a trim taken later holds only the grown object.
    ArgsA       args_a = ArgumentsA(0);
    ArgsB       args_b = ArgumentsB(1);
    ShaderTable hit(kRecordStride);
    hit.AddRecord(Identifier(original.Get(), kHitGroup), &args_a, sizeof(args_a));
    hit.AddRecord(Identifier(tile.state_object.Get(), kHitGroupAdd), &args_b, sizeof(args_b));
    SetTables(tile, Record(original.Get(), kRayGen), Record(original.Get(), kMiss), hit);
    tile.tlas              = tlas_two_.result->GetGPUVirtualAddress();
    tile.expected_instance = kAnyInstance;
}

void DxrPipelinesApp::CreateCallable()
{
    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(pipeline, DxrSceneLibrary(), { { kRayGen, nullptr }, { kMiss, nullptr } });
    AddLibrary(pipeline,
               { g_dxr_pipelines_lib, sizeof(g_dxr_pipelines_lib) },
               { { kClosestHitCall, nullptr }, { kCallable, nullptr } });
    AddHitGroup(pipeline, kHitGroupCall, kClosestHitCall);
    AddLocalRootSignature(pipeline, scene_.local_root_signature.Get(), { kHitGroupCall });
    AddLocalRootSignature(pipeline, callable_root_signature_.Get(), { kCallable });
    AddCommon(pipeline);

    TileDispatch& tile                 = tiles_[kTileCallable];
    tile.state_object                  = Create(pipeline, "CreateStateObject (callable)");
    ArgsA                     args     = ArgumentsA(0);
    D3D12_GPU_VIRTUAL_ADDRESS cbv      = callable_cbv_->GetGPUVirtualAddress();
    ShaderTable               callable = Record(tile.state_object.Get(), kCallable, &cbv, sizeof(cbv));
    SetTables(tile,
              Record(tile.state_object.Get(), kRayGen),
              Record(tile.state_object.Get(), kMiss),
              Record(tile.state_object.Get(), kHitGroupCall, &args, sizeof(args)),
              &callable);
    tile.tlas = scene_.tlas.result->GetGPUVirtualAddress();
}

void DxrPipelinesApp::CreateImplicitDefault()
{
    // Root signature A with no association is the default for RayGen, Miss and the hit group alike, so every record
    // carries its arguments.
    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(pipeline, DxrSceneLibrary(), { { kRayGen, nullptr }, { kMiss, nullptr }, { kClosestHit, nullptr } });
    AddHitGroup(pipeline, kHitGroup, kClosestHit);
    AddLocalRootSignature(pipeline, scene_.local_root_signature.Get(), {});
    AddCommon(pipeline);

    TileDispatch& tile = tiles_[kTileImplicitDefault];
    tile.state_object  = Create(pipeline, "CreateStateObject (implicit-default)");
    ArgsA args         = ArgumentsA(0);
    SetTables(tile,
              Record(tile.state_object.Get(), kRayGen, &args, sizeof(args)),
              Record(tile.state_object.Get(), kMiss, &args, sizeof(args)),
              Record(tile.state_object.Get(), kHitGroup, &args, sizeof(args)));
    tile.tlas = scene_.tlas.result->GetGPUVirtualAddress();
}

#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
void DxrPipelinesApp::CreateCollectionHitGroup()
{
    ComPtr<ID3D12StateObject> collection = CreateHitCollection(kClosestHit, scene_.local_root_signature.Get());
    CreateCollectionTile(kTileCollectionHitGroup, collection.Get(), kClosestHit, kHitGroup);
    collections_.push_back(collection);
}

void DxrPipelinesApp::CreateCollectionTwoLrs()
{
    ComPtr<ID3D12StateObject> collection_a = CreateHitCollection(kClosestHitA, scene_.local_root_signature.Get());
    ComPtr<ID3D12StateObject> collection_b = CreateHitCollection(kClosestHitB, local_root_signature_b_.Get());
    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(pipeline, DxrSceneLibrary(), { { kRayGen, nullptr }, { kMiss, nullptr } });
    auto existing_a = pipeline.CreateSubobject<CD3DX12_EXISTING_COLLECTION_SUBOBJECT>();
    existing_a->SetExistingCollection(collection_a.Get());
    auto existing_b = pipeline.CreateSubobject<CD3DX12_EXISTING_COLLECTION_SUBOBJECT>();
    existing_b->SetExistingCollection(collection_b.Get());
    AddHitGroup(pipeline, kHitGroupA, kClosestHitA);
    AddHitGroup(pipeline, kHitGroupB, kClosestHitB);
    AddCommon(pipeline);

    TileDispatch& tile = tiles_[kTileCollectionTwoLrs];
    tile.state_object  = Create(pipeline, "CreateStateObject (collection-two-lrs)");
    ArgsA       args_a = ArgumentsA(0);
    ArgsB       args_b = ArgumentsB(1);
    ShaderTable hit(kRecordStride);
    hit.AddRecord(Identifier(tile.state_object.Get(), kHitGroupA), &args_a, sizeof(args_a));
    hit.AddRecord(Identifier(tile.state_object.Get(), kHitGroupB), &args_b, sizeof(args_b));
    SetTables(tile, Record(tile.state_object.Get(), kRayGen), Record(tile.state_object.Get(), kMiss), hit);
    tile.tlas              = tlas_two_.result->GetGPUVirtualAddress();
    tile.expected_instance = kAnyInstance;
    collections_.push_back(collection_a);
    collections_.push_back(collection_b);
}

void DxrPipelinesApp::CreateCollectionReleased()
{
    // The collection goes out of scope here: the pipeline holds the runtime's reference, a trim's state block none.
    ComPtr<ID3D12StateObject> collection = CreateHitCollection(kClosestHitR, scene_.local_root_signature.Get());
    CreateCollectionTile(kTileCollectionReleased, collection.Get(), kClosestHitR, kHitGroupR);
}

void DxrPipelinesApp::CreateDxilDefinedLrs()
{
    CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
    AddLibrary(pipeline, DxrSceneLibrary(), { { kRayGen, nullptr }, { kMiss, nullptr } });
    AddLibrary(pipeline,
               { g_dxr_pipelines_dxil_lrs_lib, sizeof(g_dxr_pipelines_dxil_lrs_lib) },
               { { kClosestHitDxil, nullptr }, { kDxilLrs, nullptr }, { kDxilLrsAssoc, nullptr } });
    AddHitGroup(pipeline, kHitGroupDxil, kClosestHitDxil);
    AddCommon(pipeline);

    TileDispatch& tile = tiles_[kTileDxilDefinedLrs];
    tile.state_object  = Create(pipeline, "CreateStateObject (dxil-defined-lrs)");
    ArgsA args         = ArgumentsA(0);
    SetTables(tile,
              Record(tile.state_object.Get(), kRayGen),
              Record(tile.state_object.Get(), kMiss),
              Record(tile.state_object.Get(), kHitGroupDxil, &args, sizeof(args)));
    tile.tlas = scene_.tlas.result->GetGPUVirtualAddress();
}
#endif

void DxrPipelinesApp::Initialize()
{
    scene_ = CreateDxrScene();

    // A TLAS with two instances side by side: A over the left half of the tile, B over the right.
    D3D12_RAYTRACING_INSTANCE_DESC descs[2] = { scene_.InstanceDesc(kInstanceIdA, 0),
                                                scene_.InstanceDesc(kInstanceIdB, 1) };
    for (uint32_t i = 0; i < 2; ++i)
    {
        descs[i].Transform[0][0] = 0.5f;
        descs[i].Transform[0][3] = (i == 0) ? -0.6f : 0.6f;
    }
    two_instance_descs_ = CreateUploadBuffer(descs, sizeof(descs), L"two instance descs");
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs =
        DxrScene::TlasInputs(two_instance_descs_->GetGPUVirtualAddress());
    inputs.NumDescs = 2;
    tlas_two_       = CreateAccelerationStructure(inputs, L"TLAS two instances");
    BuildAccelerationStructure(tlas_two_, inputs);

    // Local root signature B: the scene's three parameters in the order table, CBV, constant (ArgsB).
    D3D12_DESCRIPTOR_RANGE local_range = {
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 1, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    };
    D3D12_ROOT_PARAMETER params_b[3] = {};
    params_b[0].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    params_b[0].DescriptorTable      = { 1, &local_range };
    params_b[1].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_CBV;
    params_b[1].Descriptor           = { 1, 1 };
    params_b[2].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    params_b[2].Constants            = { 0, 1, 1 };
    D3D12_ROOT_SIGNATURE_DESC desc_b = {};
    desc_b.NumParameters             = 3;
    desc_b.pParameters               = params_b;
    desc_b.Flags                     = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
    local_root_signature_b_          = CreateRootSignature(desc_b);

    // The callable shader's local root signature: a root CBV (b0, space2) over a buffer holding kCallableMagic.
    D3D12_ROOT_PARAMETER params_c[1] = {};
    params_c[0].ParameterType        = D3D12_ROOT_PARAMETER_TYPE_CBV;
    params_c[0].Descriptor           = { 0, 2 };
    D3D12_ROOT_SIGNATURE_DESC desc_c = {};
    desc_c.NumParameters             = 1;
    desc_c.pParameters               = params_c;
    desc_c.Flags                     = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
    callable_root_signature_         = CreateRootSignature(desc_c);
    uint32_t callable_data[64]       = { kCallableMagic };
    callable_cbv_                    = CreateUploadBuffer(callable_data, sizeof(callable_data), L"callable CBV");

    CreateCollectionExplicit();
    CreateAddToStateObject();
    CreateCallable();
    CreateImplicitDefault();
#ifdef GFXRECON_TESTAPP_KNOWN_DEFECT_TILES
    CreateCollectionHitGroup();
    CreateCollectionTwoLrs();
    CreateCollectionReleased();
    CreateDxilDefinedLrs();
#endif
}

void DxrPipelinesApp::RecordFrame(uint32_t frame, uint32_t slot)
{
    GFXRECON_UNREFERENCED_PARAMETER(frame);
    GFXRECON_UNREFERENCED_PARAMETER(slot);
    ID3D12GraphicsCommandList4* list = List();
    list->SetComputeRootSignature(scene_.global_root_signature.Get());
    list->SetComputeRootDescriptorTable(1, OutputUav().gpu);
    for (uint32_t tile = 0; tile < kTileCount; ++tile)
    {
        if (!TileEnabled(tile))
        {
            continue;
        }
        const TileDispatch& dispatch  = tiles_[tile];
        TileConstants       constants = { TileX(tile), TileY(tile), kTileSize, dispatch.expected_instance };
        list->SetComputeRoot32BitConstants(0, kTileConstantCount, &constants, 0);
        list->SetComputeRootShaderResourceView(2, dispatch.tlas);
        list->SetPipelineState1(dispatch.state_object.Get());
        list->DispatchRays(&dispatch.desc);
    }
}

GFXRECON_D3D12_TEST_APP_MAIN(DxrPipelinesApp)
