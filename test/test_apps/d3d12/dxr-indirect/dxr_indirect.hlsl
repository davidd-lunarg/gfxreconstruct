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

// main_desc: writes a D3D12_DISPATCH_RAYS_DESC into the argument buffer from root constants, so the desc the
// ExecuteIndirect reads was composed by the GPU. Offsets: raygen {start, size} 0, miss {start, size, stride} 16,
// hit group 40, callable 64 (zero), width / height / depth 88.

cbuffer DescParams : register(b0)
{
    uint2 raygen_start;
    uint  raygen_size;
    uint  desc_pad0;
    uint2 miss_start;
    uint  miss_size;
    uint  miss_stride;
    uint2 hit_start;
    uint  hit_size;
    uint  hit_stride;
    uint  dispatch_width;
    uint  dispatch_height;
    uint  desc_pad1;
    uint  desc_pad2;
};

RWByteAddressBuffer g_args : register(u0);

[numthreads(1, 1, 1)]
void main_desc(uint3 id : SV_DispatchThreadID)
{
    g_args.Store4(0, uint4(raygen_start, raygen_size, 0));
    g_args.Store4(16, uint4(miss_start, miss_size, 0));
    g_args.Store2(32, uint2(miss_stride, 0));
    g_args.Store4(40, uint4(hit_start, hit_size, 0));
    g_args.Store2(56, uint2(hit_stride, 0));
    g_args.Store4(64, uint4(0, 0, 0, 0));
    g_args.Store2(80, uint2(0, 0));
    g_args.Store4(88, uint4(dispatch_width, dispatch_height, 1, 0));
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
