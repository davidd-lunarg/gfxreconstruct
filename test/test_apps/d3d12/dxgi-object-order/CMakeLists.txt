add_executable(gfxrecon-testapp-d3d12-dxgi-object-order main.cpp)
target_compile_definitions(gfxrecon-testapp-d3d12-dxgi-object-order PRIVATE UNICODE _UNICODE)
target_link_libraries(gfxrecon-testapp-d3d12-dxgi-object-order PRIVATE d3d12.lib dxgi.lib)
