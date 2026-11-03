// dxr-basic: inline raytracing (RayQuery) against the TLAS bound through an SRV descriptor. Fail bits: 8 the committed
// instance is not the expected one, 16 no triangle hit.

struct TileConstants
{
    uint tile_x;
    uint tile_y;
    uint tile_size;
    uint expected_instance;
};

ConstantBuffer<TileConstants>   g_tile  : register(b0);
RWTexture2D<float4>             g_out   : register(u0);
RaytracingAccelerationStructure g_scene : register(t1);

float4 Verdict(uint fail_bits)
{
    return (fail_bits == 0) ? float4(0, 1, 0, 1) : float4(1, 0, (fail_bits * 8) / 255.0, 1);
}

[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= g_tile.tile_size || id.y >= g_tile.tile_size)
    {
        return;
    }
    float2 uv = (float2(id.xy) + 0.5) / float(g_tile.tile_size);
    RayDesc ray;
    ray.Origin    = float3(uv * 2.0 - 1.0, 0.0);
    ray.Direction = float3(0.0, 0.0, 1.0);
    ray.TMin      = 0.0;
    ray.TMax      = 10.0;
    RayQuery<RAY_FLAG_NONE> query;
    query.TraceRayInline(g_scene, RAY_FLAG_NONE, 0xFF, ray);
    query.Proceed();
    uint bits = 0;
    if (query.CommittedStatus() != COMMITTED_TRIANGLE_HIT)
    {
        bits |= 16;
    }
    else if (query.CommittedInstanceID() != g_tile.expected_instance)
    {
        bits |= 8;
    }
    g_out[uint2(g_tile.tile_x, g_tile.tile_y) + id.xy] = Verdict(bits);
}
