add_executable(gfxrecon-testapp-d3d12-dxr-basic dxr_basic.cpp)
target_link_libraries(gfxrecon-testapp-d3d12-dxr-basic PRIVATE gfxrecon-testapp-d3d12-common)
gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-dxr-basic ${CMAKE_CURRENT_SOURCE_DIR}/dxr_basic_inline.hlsl cs_6_5 main
                      g_dxr_basic_inline_cs)

# D3D12Core.dll and d3d12SDKLayers.dll next to the exe, under D3D12/, as for gfxrecon-replay.
include(${PROJECT_SOURCE_DIR}/cmake/AgilitySDK.cmake)
