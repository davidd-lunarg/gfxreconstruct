// The DXR library of dxr-pipelines beside the shared scene library (dxr_scene.hlsl, the same bindings): a closest hit
// shader that calls a callable shader, the callable, which returns the value behind the root CBV of its own local
// root signature (b0, space2), and a raygen shader that checks its own record's arguments. Fail bits as
// dxr_scene.hlsl; the callable returning the wrong value sets 2 (root CBV).

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

cbuffer CallableCbv : register(b0, space2)
{
    uint g_callable_cbv;
};

struct Payload
{
    uint fail_bits;
};

struct CallParams
{
    uint value;
};

float4 Verdict(uint fail_bits)
{
    return (fail_bits == 0) ? float4(0, 1, 0, 1) : float4(1, 0, (fail_bits * 8) / 255.0, 1);
}

[shader("closesthit")]
void ClosestHitCall(inout Payload payload, in BuiltInTriangleIntersectionAttributes attributes)
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
    CallParams params;
    params.value = 0;
    CallShader(0, params);
    if (params.value != 0xD0D0D0D0)
    {
        bits |= 2;
    }
    payload.fail_bits = bits;
}

[shader("callable")]
void Callable(inout CallParams params)
{
    params.value = g_callable_cbv;
}

// The scene's raygen, checking the arguments of its own record as well, for a pipeline whose default local root
// signature gives the raygen record arguments (tile 7): the values expected for the tile's instance.
[shader("raygeneration")]
void RayGenLocal()
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
    uint id   = g_tile.expected_instance;
    uint bits = payload.fail_bits;
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
    g_out[uint2(g_tile.tile_x, g_tile.tile_y) + index] = Verdict(bits);
}
