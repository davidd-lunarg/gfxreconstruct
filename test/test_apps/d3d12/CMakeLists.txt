# D3D12 test apps for capture and replay of DX12 and DXR. They are standalone executables, independent of the Vulkan
# test apps (GFXRECON_INCLUDE_TEST_APPS). The DXR apps compile their shaders at build time with the DXC SDK's dxc.exe
# (cmake/FindDXC.cmake) and are skipped without it.

# Tiles whose DXR addresses are written by the GPU replay correctly only with a translation that follows GPU writes.
# Without one, replay hands the GPU capture-time addresses, which can hang it, so these tiles are left out of the build
# by default. A tile left out stays gray and is reported "not built".
option(GFXRECON_D3D12_TESTAPP_GPU_WRITTEN_TILES
       "Build the D3D12 test app tiles whose DXR addresses are written on the GPU" OFF)

# Tiles that reproduce known replay defects fail on a build without the fix, and some leave shader records
# untranslated, which can fault the GPU. Turn this on to test a fix for one of them.
option(GFXRECON_D3D12_TESTAPP_KNOWN_DEFECT_TILES
       "Build the D3D12 test app tiles that reproduce known replay defects" OFF)

# Compile one entry point of <hlsl> (pass "" for a library profile) into a header that defines the byte array
# <variable>, and add the header to <target>'s sources.
function(gfxrecon_d3d12_shader target hlsl profile entry variable)
    set(shader_dir ${CMAKE_CURRENT_BINARY_DIR}/shaders)
    set(header ${shader_dir}/${variable}.h)
    set(entry_args "")
    if (NOT entry STREQUAL "")
        set(entry_args -E ${entry})
    endif ()
    add_custom_command(OUTPUT ${header}
                       COMMAND ${CMAKE_COMMAND} -E make_directory ${shader_dir}
                       COMMAND ${DXC_EXE_PATH} -nologo -T ${profile} ${entry_args} -O3 -Fh ${header} -Vn ${variable}
                               ${hlsl}
                       DEPENDS ${hlsl}
                       COMMENT "Compiling ${hlsl} (${profile} ${entry})"
                       VERBATIM)
    target_sources(${target} PRIVATE ${header})
    target_include_directories(${target} PRIVATE ${shader_dir})
endfunction()

add_subdirectory(dxgi-object-order)

if (DXC_EXE_PATH)
    add_subdirectory(common)
    if (GFXRECON_D3D12_TESTAPP_GPU_WRITTEN_TILES)
        target_compile_definitions(gfxrecon-testapp-d3d12-common PUBLIC GFXRECON_TESTAPP_GPU_WRITTEN_TILES)
    endif ()
    if (GFXRECON_D3D12_TESTAPP_KNOWN_DEFECT_TILES)
        target_compile_definitions(gfxrecon-testapp-d3d12-common PUBLIC GFXRECON_TESTAPP_KNOWN_DEFECT_TILES)
    endif ()
    add_subdirectory(dxr-basic)
    add_subdirectory(dxr-indirect)
    add_subdirectory(dxr-acceleration-structures)
    add_subdirectory(dxr-pipelines)
    add_subdirectory(dxr-command-contexts)
    message(STATUS "D3D12 DXR test apps: ${DXC_EXE_PATH}")
else ()
    message(STATUS "D3D12 DXR test apps: dxc.exe not found, skipped")
endif ()
