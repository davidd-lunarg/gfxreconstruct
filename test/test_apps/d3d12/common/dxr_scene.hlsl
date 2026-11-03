// The DXR library of the shared scene (dxr_scene.h). Every ray of a tile hits the quad at z = 1; the closest hit
// shader checks what the hit group's shader record carries (a root constant, a root CBV, a descriptor table) against
// the values expected for the instance it hit, and the raygen shader writes the tile's verdict. Fail bits: 1 root
// constant, 2 root CBV, 4 descriptor table, 8 InstanceID (unless the expected instance is 0xFFFFFFFF, any), 16 miss.

struct TileConstants
{
    uint tile_x;
    uint tile_y;
    uint tile_size;
    uint expected_instance;
};

ConstantBuffer<TileConstants>   g_tile  : register(b0);
RWTexture2D<float4>             g_out   : register(u0);
RaytracingAccelerationStructure g_scene : register(t0);

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

float4 Verdict(uint fail_bits)
{
    return (fail_bits == 0) ? float4(0, 1, 0, 1) : float4(1, 0, (fail_bits * 8) / 255.0, 1);
}

[shader("raygeneration")]
void RayGen()
{
    uint2  index = DispatchRaysIndex().xy;
    float2 uv    = (float2(index) + 0.5) / float2(DispatchRaysDimensions().xy);
    RayDesc ray;
    ray.Origin    = float3(uv * 2.0 - 1.0, 0.0);
    ray.Direction = float3(0.0, 0.0, 1.0);
    ray.TMin      = 0.0;
    ray.TMax      = 10.0;
    Payload payload;
    payload.fail_bits = 0;
    TraceRay(g_scene, RAY_FLAG_NONE, 0xFF, 0, 1, 0, ray, payload);
    g_out[uint2(g_tile.tile_x, g_tile.tile_y) + index] = Verdict(payload.fail_bits);
}

[shader("closesthit")]
void ClosestHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attributes)
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

[shader("miss")]
void Miss(inout Payload payload)
{
    payload.fail_bits = 16;
}
