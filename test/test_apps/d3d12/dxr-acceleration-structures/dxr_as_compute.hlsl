// dxr-acceleration-structures: the compute shaders that compose top-level build inputs on the GPU from root constants.
// Root constants are not translated at replay, so what these write holds capture addresses unless the replay
// translates values written on the GPU. Each entry point is compiled on its own (CMakeLists.txt).

// main_write_descs: `count` instance descs, band b with the band transform (dxr_acceleration_structures.cpp BandDesc),
// InstanceID b, mask 0xFF, no flags, and the BLAS of b's parity. Desc layout: Transform[3][4] at 0, InstanceID:24 |
// InstanceMask:8 at 48, InstanceContributionToHitGroupIndex:24 | Flags:8 at 52, AccelerationStructure at 56.

cbuffer DescValues : register(b0)
{
    uint2 g_blas_even;
    uint2 g_blas_odd;
    uint  g_count;
    float g_scale; // the band transform: y scaled by g_scale, offset g_y0 + g_dy * band
    float g_y0;
    float g_dy;
};

RWByteAddressBuffer g_descs : register(u0);

[numthreads(4, 1, 1)]
void main_write_descs(uint3 id : SV_DispatchThreadID)
{
    uint b = id.x;
    if (b >= g_count)
    {
        return;
    }
    uint  base  = b * 64;
    float scale = g_scale;
    float ty    = g_y0 + g_dy * float(b);
    uint2 blas  = ((b & 1) != 0) ? g_blas_odd : g_blas_even;
    g_descs.Store4(base, asuint(float4(1.0, 0.0, 0.0, 0.0)));
    g_descs.Store4(base + 16, asuint(float4(0.0, scale, 0.0, ty)));
    g_descs.Store4(base + 32, asuint(float4(0.0, 0.0, 1.0, 0.0)));
    g_descs.Store4(base + 48, uint4(b | (0xFFu << 24), 0, blas.x, blas.y));
}

// main_write_pointers: an instance desc pointer array of four entries.

cbuffer PointerValues : register(b1)
{
    uint4 g_pointers_01; // pointer 0 lo, hi, pointer 1 lo, hi
    uint4 g_pointers_23;
};

RWByteAddressBuffer g_pointers : register(u1);

[numthreads(1, 1, 1)]
void main_write_pointers(uint3 id : SV_DispatchThreadID)
{
    g_pointers.Store4(0, g_pointers_01);
    g_pointers.Store4(16, g_pointers_23);
}
