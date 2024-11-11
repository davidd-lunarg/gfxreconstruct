###############################################################################
# Copyright (c) 2018-2024 LunarG, Inc.
# Copyright (c) 2020-2022 Advanced Micro Devices, Inc.
# All rights reserved
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#
# Author: LunarG Team
# Author: AMD Developer Tools Team
# Description: CMake script for framework util target
###############################################################################

add_executable(gfxrecon-trim "")

target_compile_definitions(gfxrecon-trim PUBLIC GFXRECON_TRIM_TOOL)

set(CMAKE_REPLAY_TOOL_SRC_DIR "${CMAKE_SOURCE_DIR}/tools/replay")

target_sources(gfxrecon-trim
               PRIVATE
                    ${CMAKE_REPLAY_TOOL_SRC_DIR}/../tool_settings.h
                    ${CMAKE_REPLAY_TOOL_SRC_DIR}/replay_settings.h
                    ${CMAKE_REPLAY_TOOL_SRC_DIR}/desktop_main.cpp
                    ${CMAKE_REPLAY_TOOL_SRC_DIR}/parse_dump_resources_cli.cpp
                    ${CMAKE_REPLAY_TOOL_SRC_DIR}/../platform_debug_helper.cpp
                    $<$<BOOL:WIN32>:${CMAKE_SOURCE_DIR}/version.rc>
)

target_include_directories(gfxrecon-trim PUBLIC ${CMAKE_BINARY_DIR})

target_link_libraries(gfxrecon-trim
                          gfxrecon_application
                          gfxrecon_decode
                          gfxrecon_encode
                          platform_specific
                          $<$<BOOL:${D3D12_SUPPORT}>:d3d12.lib>
                          $<$<BOOL:${D3D12_SUPPORT}>:dxgi.lib>
                          $<$<BOOL:${DXC_FOUND}>:${DXC_LIBRARY_PATH}>)

target_link_options(gfxrecon-replay PUBLIC "-rdynamic")

if (MSVC)
    # Force inclusion of "gfxrecon_disable_popup_result" variable in linking.
    # On 32-bit windows, MSVC prefixes symbols with "_" but on 64-bit windows it doesn't.
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
      target_link_options(gfxrecon-trim PUBLIC "LINKER:/Include:_gfxrecon_disable_popup_result")
    else()
      target_link_options(gfxrecon-trim PUBLIC "LINKER:/Include:gfxrecon_disable_popup_result")
    endif()
endif()

# A multithreaded Vulkan driver loaded through dlopen() mechanism
# won't work if libstdc++ has started in a single-threaded mode.
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(gfxrecon-trim Threads::Threads)

common_build_directives(gfxrecon-trim)

include(${CMAKE_SOURCE_DIR}/cmake/AgilitySDK.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/InstallDXC.cmake)

install(TARGETS gfxrecon-trim RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

