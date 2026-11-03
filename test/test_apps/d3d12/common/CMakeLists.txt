add_library(gfxrecon-testapp-d3d12-common STATIC d3d12_test_app.h d3d12_test_app.cpp dxr_scene.h dxr_scene.cpp)
target_include_directories(gfxrecon-testapp-d3d12-common
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                  ${PROJECT_SOURCE_DIR}/framework
                                  ${D3D12_INCLUDE_DIRECTORIES})
target_compile_definitions(gfxrecon-testapp-d3d12-common PUBLIC UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
target_link_libraries(gfxrecon-testapp-d3d12-common PUBLIC d3d12.lib dxgi.lib dxguid.lib)
gfxrecon_d3d12_shader(gfxrecon-testapp-d3d12-common ${CMAKE_CURRENT_SOURCE_DIR}/dxr_scene.hlsl lib_6_3 ""
                      g_dxr_scene_lib)
