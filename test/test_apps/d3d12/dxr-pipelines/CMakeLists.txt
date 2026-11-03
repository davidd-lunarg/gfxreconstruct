add_executable(gfxrecon-testapp-d3d12-dxr-pipelines dxr_pipelines.cpp)
target_link_libraries(gfxrecon-testapp-d3d12-dxr-pipelines PRIVATE gfxrecon-testapp-d3d12-common)
gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-pipelines ${CMAKE_CURRENT_SOURCE_DIR}/dxr_pipelines.hlsl lib_6_3 ""
                      g_dxr_pipelines_lib)
if (GFXRECON_D3D12_TESTAPP_KNOWN_DEFECT_TILES)
    gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-pipelines ${CMAKE_CURRENT_SOURCE_DIR}/dxr_pipelines_dxil_lrs.hlsl
                          lib_6_3 "" g_dxr_pipelines_dxil_lrs_lib)
endif ()

# D3D12Core.dll and d3d12SDKLayers.dll next to the exe, under D3D12/, as for gfxrecon-replay.
include(${PROJECT_SOURCE_DIR}/cmake/AgilitySDK.cmake)
