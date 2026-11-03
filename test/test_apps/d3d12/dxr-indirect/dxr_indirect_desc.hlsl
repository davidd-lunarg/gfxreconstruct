// dxr-indirect: the compute shader of tile 1, in a file of its own: its bindings (b0, u0) are the ones main_views
// (dxr_indirect.hlsl) uses for other resources, and each entry point is compiled from its own file.

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
