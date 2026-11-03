// The DXR library of dxr-pipelines beside the shared scene library (dxr_scene.hlsl, the same bindings): a closest hit
// shader that calls a callable shader, and the callable, which returns the value behind the root CBV of its own local
// root signature (b0, space2). Fail bits as dxr_scene.hlsl; the callable returning the wrong value sets 2 (root CBV).

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
