add_executable(gfxrecon-testapp-d3d12-dxr-indirect dxr_indirect.cpp)
target_link_libraries(gfxrecon-testapp-d3d12-dxr-indirect PRIVATE gfxrecon-testapp-d3d12-common)
set(hlsl ${CMAKE_CURRENT_SOURCE_DIR}/dxr_indirect.hlsl)
if (GFXRECON_D3D12_TESTAPP_GPU_WRITTEN_TILES)
    gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-indirect ${CMAKE_CURRENT_SOURCE_DIR}/dxr_indirect_desc.hlsl
                          cs_6_0 main_desc g_dxr_indirect_desc_cs)
endif ()
gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-indirect ${hlsl} cs_6_0 main_views g_dxr_indirect_views_cs)
gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-indirect ${hlsl} vs_6_0 vs_quad g_dxr_indirect_quad_vs)
gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-indirect ${hlsl} ps_6_0 ps_quad g_dxr_indirect_quad_ps)

# D3D12Core.dll and d3d12SDKLayers.dll next to the exe, under D3D12/, as for gfxrecon-replay.
include(${PROJECT_SOURCE_DIR}/cmake/AgilitySDK.cmake)
