add_executable(gfxrecon-testapp-d3d12-dxr-acceleration-structures dxr_acceleration_structures.cpp)
target_link_libraries(gfxrecon-testapp-d3d12-dxr-acceleration-structures PRIVATE gfxrecon-testapp-d3d12-common)
gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-acceleration-structures ${CMAKE_CURRENT_SOURCE_DIR}/dxr_as.hlsl
                      lib_6_3 "" g_dxr_as_lib)
if (GFXRECON_D3D12_TESTAPP_GPU_WRITTEN_TILES)
    set(compute ${CMAKE_CURRENT_SOURCE_DIR}/dxr_as_compute.hlsl)
    gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-acceleration-structures ${compute} cs_6_0 main_write_descs
                          g_dxr_as_write_descs_cs)
    gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-acceleration-structures ${compute} cs_6_0 main_write_pointers
                          g_dxr_as_write_pointers_cs)
endif ()

# D3D12Core.dll and d3d12SDKLayers.dll next to the exe, under D3D12/, as for gfxrecon-replay.
include(${PROJECT_SOURCE_DIR}/cmake/AgilitySDK.cmake)
