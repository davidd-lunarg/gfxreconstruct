// dxr-indirect: the shaders beside the shared DXR library (dxr_scene.hlsl). Each entry point is compiled on its own
// (dxr-indirect/CMakeLists.txt).

struct TileConstants
{
    uint tile_x;
    uint tile_y;
    uint tile_size;
    uint expected_instance;
};

float4 Verdict(uint fail_bits)
{
    return (fail_bits == 0) ? float4(0, 1, 0, 1) : float4(1, 0, (fail_bits * 8) / 255.0, 1);
}

// main_views: reads a magic value through each root view the command signature delivered and paints the tile.
// Fail bits: 1 root CBV, 2 root SRV, 4 root UAV.

ConstantBuffer<TileConstants> g_tile : register(b0);
RWTexture2D<float4>           g_out  : register(u0);
cbuffer ViewMagic : register(b1)
{
    uint g_cbv_magic;
};
ByteAddressBuffer   g_srv : register(t0);
RWByteAddressBuffer g_uav : register(u1);

[numthreads(8, 8, 1)]
void main_views(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= g_tile.tile_size || id.y >= g_tile.tile_size)
    {
        return;
    }
    uint bits = 0;
    if (g_cbv_magic != 0xC0FFEE01)
    {
        bits |= 1;
    }
    if (g_srv.Load(0) != 0xC0FFEE02)
    {
        bits |= 2;
    }
    if (g_uav.Load(0) != 0xC0FFEE03)
    {
        bits |= 4;
    }
    g_out[uint2(g_tile.tile_x, g_tile.tile_y) + id.xy] = Verdict(bits);
}

// vs_quad / ps_quad: a quad drawn into the tile (the viewport) from a vertex and an index buffer the command
// signature delivered; the pixel shader checks the root CBV it delivered too. Fail bits: 1 root CBV. A wrong vertex or
// index buffer draws nothing, leaving the tile black.

struct VsInput
{
    float3 position : POSITION;
};

struct VsOutput
{
    float4 position : SV_Position;
};

VsOutput vs_quad(VsInput input)
{
    VsOutput output;
    output.position = float4(input.position, 1.0);
    return output;
}

float4 ps_quad(VsOutput input) : SV_Target
{
    return Verdict((g_cbv_magic != 0xC0FFEE04) ? 1 : 0);
}
