add_executable(gfxrecon-testapp-d3d12-dxr-command-contexts dxr_command_contexts.cpp)
target_link_libraries(gfxrecon-testapp-d3d12-dxr-command-contexts PRIVATE gfxrecon-testapp-d3d12-common)

# D3D12Core.dll and d3d12SDKLayers.dll next to the exe, under D3D12/, as for gfxrecon-replay.
include(${PROJECT_SOURCE_DIR}/cmake/AgilitySDK.cmake)
