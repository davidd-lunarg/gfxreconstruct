// dxr-acceleration-structures: the DXR library. A tile is four horizontal bands; band b is an instance of a quad scaled
// into the band's rows, with InstanceID b and the BLAS of b's parity (even bands one quad depth, odd bands another).
// The closest hit shader reports the instance it hit and the hit distance, which is the quad's z and so names the
// BLAS; the raygen shader judges the band against the tile's constants. Fail bits: 1 a hit on an inactive band,
// 2 wrong hit distance (another BLAS), 4 InstanceIndex, 8 InstanceID, 16 miss on an active band.

struct BandConstants
{
    uint  tile_x;
    uint  tile_y;
    uint  tile_size;
    uint  active_mask; // bit b: band b holds an active instance and every ray of it must hit
    float z_even;      // the hit distance expected on even bands
    float z_odd;       // and on odd bands
    uint  pad0;
    uint  pad1;
};

ConstantBuffer<BandConstants>   g_tile  : register(b0);
RWTexture2D<float4>             g_out   : register(u0);
RaytracingAccelerationStructure g_scene : register(t0);

static const uint kBands = 4;

struct Payload
{
    uint  hit;
    uint  instance_id;
    uint  instance_index;
    float t;
};

float4 Verdict(uint fail_bits)
{
    return (fail_bits == 0) ? float4(0, 1, 0, 1) : float4(1, 0, (fail_bits * 8) / 255.0, 1);
}

[shader("raygeneration")]
void RayGen()
{
    uint2  index = DispatchRaysIndex().xy;
    uint2  dims  = DispatchRaysDimensions().xy;
    float2 uv    = (float2(index) + 0.5) / float2(dims);
    uint   band  = (index.y * kBands) / dims.y;
    RayDesc ray;
    ray.Origin    = float3(uv * 2.0 - 1.0, 0.0);
    ray.Direction = float3(0.0, 0.0, 1.0);
    ray.TMin      = 0.0;
    ray.TMax      = 10.0;
    Payload payload;
    payload.hit            = 0;
    payload.instance_id    = 0;
    payload.instance_index = 0;
    payload.t              = 0.0;
    TraceRay(g_scene, RAY_FLAG_NONE, 0xFF, 0, 1, 0, ray, payload);

    uint bits = 0;
    if ((g_tile.active_mask & (1u << band)) == 0)
    {
        if (payload.hit != 0)
        {
            bits |= 1;
        }
    }
    else if (payload.hit == 0)
    {
        bits |= 16;
    }
    else
    {
        float expected = ((band & 1) != 0) ? g_tile.z_odd : g_tile.z_even;
        if (abs(payload.t - expected) > 0.01)
        {
            bits |= 2;
        }
        if (payload.instance_index != band)
        {
            bits |= 4;
        }
        if (payload.instance_id != band)
        {
            bits |= 8;
        }
    }
    g_out[uint2(g_tile.tile_x, g_tile.tile_y) + index] = Verdict(bits);
}

[shader("closesthit")]
void ClosestHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attributes)
{
    payload.hit            = 1;
    payload.instance_id    = InstanceID();
    payload.instance_index = InstanceIndex();
    payload.t              = RayTCurrent();
}

[shader("miss")]
void Miss(inout Payload payload)
{
    payload.hit = 0;
}
