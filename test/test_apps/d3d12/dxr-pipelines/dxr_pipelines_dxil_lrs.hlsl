// The closest hit shader of the dxil-defined-lrs tile with its local root signature and the association declared as
// library subobjects, so the state object description carries no LOCAL_ROOT_SIGNATURE subobject for it. The root
// signature is the scene's (dxr_scene.h): a root constant, a root CBV and a descriptor table in space1, which gives the
// hit record the scene's layout. The checks are dxr_scene.hlsl's.

LocalRootSignature DxilLocalRootSignature =
{
    "RootConstants(num32BitConstants = 1, b0, space = 1), CBV(b1, space = 1), DescriptorTable(SRV(t0, space = 1))"
};

SubobjectToExportsAssociation DxilLocalRootSignatureAssociation =
{
    "DxilLocalRootSignature",
    "ClosestHitDxil"
};

struct TileConstants
{
    uint tile_x;
    uint tile_y;
    uint tile_size;
    uint expected_instance;
};

ConstantBuffer<TileConstants> g_tile : register(b0);

cbuffer LocalConstants : register(b0, space1)
{
    uint g_local_constant;
};
cbuffer LocalCbv : register(b1, space1)
{
    uint g_local_cbv;
};
ByteAddressBuffer g_local_table : register(t0, space1);

struct Payload
{
    uint fail_bits;
};

[shader("closesthit")]
void ClosestHitDxil(inout Payload payload, in BuiltInTriangleIntersectionAttributes attributes)
{
    uint id   = InstanceID();
    uint bits = 0;
    if (g_tile.expected_instance != 0xFFFFFFFF && id != g_tile.expected_instance)
    {
        bits |= 8;
    }
    if (g_local_constant != (0xA000 | id))
    {
        bits |= 1;
    }
    if (g_local_cbv != (0xB000 | id))
    {
        bits |= 2;
    }
    if (g_local_table.Load(0) != (0xC000 | id))
    {
        bits |= 4;
    }
    payload.fail_bits = bits;
}
