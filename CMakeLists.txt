cmake_minimum_required(VERSION 3.20)

project(qpr2_recomp LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT WIN32)
    message(FATAL_ERROR
        "The first QPR2 bring-up target is Windows 10/11 with the XboxRecomp "
        "D3D11 backend. The analysis/lift pipeline can still run on Linux/macOS.")
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

set(XBOXRECOMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xboxrecomp"
    CACHE PATH "Path to the xboxrecomp toolkit")

if(NOT EXISTS "${XBOXRECOMP_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "XboxRecomp was not found at ${XBOXRECOMP_DIR}. Run: "
        "git submodule update --init --recursive")
endif()

file(GLOB RECOMP_GEN_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/recomp/gen/*.c")

if(NOT RECOMP_GEN_SOURCES)
    message(FATAL_ERROR
        "No recompiled QPR2 code was found. Put your own default.xbe in game/ "
        "and run: py -3 tools/qpr2_pipeline.py all")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/qpr2_build_config.h")
    message(FATAL_ERROR
        "QPR2 build metadata is missing. Run: py -3 tools/qpr2_pipeline.py analyze")
endif()

# Re-embed shader sources whenever HLSL, shared layout or scalar kernel changes.
# The generated header is checked in for tools; the build keeps it current.
set(QPR2_SHADER_ROOT "${XBOXRECOMP_DIR}/src/kernel")
set(QPR2_SHADER_INPUTS)
foreach(name nv2a_gpu_raster.hlsl nv2a_gpu_rov.hlsl nv2a_gpu_report_reset.hlsl
             nv2a_gpu_coverage.hlsl nv2a_gpu_clear.hlsl nv2a_gpu_types.h
             nv2a_gpu_pixel.inc nv2a_gpu_literal.inc nv2a_gpu_compact.inc nv2a_gpu_shade.inc nv2a_gpu_source.inc nv2a_gpu_geometry.hlsl nv2a_gpu_texcopy.hlsl
             nv2a_gpu_raw_types.h nv2a_gpu_vsh.inc nv2a_gpu_prepare.inc)
    list(APPEND QPR2_SHADER_INPUTS "${QPR2_SHADER_ROOT}/${name}")
endforeach()
add_custom_command(OUTPUT "${QPR2_SHADER_ROOT}/nv2a_gpu_shader.h"
    COMMAND "${CMAKE_COMMAND}" "-DINPUT_ROOT=${QPR2_SHADER_ROOT}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/tools/embed_qpr2_gpu_shader.cmake"
    DEPENDS ${QPR2_SHADER_INPUTS} "${CMAKE_CURRENT_SOURCE_DIR}/tools/embed_qpr2_gpu_shader.cmake"
    VERBATIM)
add_custom_target(qpr2_gpu_shader_sources DEPENDS "${QPR2_SHADER_ROOT}/nv2a_gpu_shader.h")

add_subdirectory("${XBOXRECOMP_DIR}" "${CMAKE_BINARY_DIR}/xboxrecomp")

add_dependencies(xbox_kernel qpr2_gpu_shader_sources)
add_dependencies(xboxrecomp qpr2_gpu_shader_sources)

add_executable(${PROJECT_NAME} WIN32
    src/main.c
    src/recomp_manual.c
    src/qpr2_recovered.c
    src/qpr2_callbacks.c
    src/qpr2_v26_callbacks.c
    src/qpr2_v28_callbacks.c
    src/qpr2_v29_callbacks.c
    src/qpr2_v31_callbacks.c
    src/qpr2_v33_callbacks.c
    src/qpr2_v34_callbacks.c
    src/qpr2_v35_callbacks.c
    src/qpr2_v36_callbacks.c
    src/qpr2_v43_callbacks.c
    src/qpr2_v42_callbacks.c
    src/qpr2_v41_callbacks.c
    src/qpr2_v37_callbacks.c
    src/qpr2_game_timing.c
    src/qpr2_menu_probe.c
    src/qpr2_ui_trace.c
    src/qpr2_xmv_probe.c
    ${RECOMP_GEN_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    xboxrecomp
    d3d11
    dxgi
    dxguid
    xinput
    winmm
    dbghelp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/generated"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/recomp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/recomp/gen"
    "${XBOXRECOMP_DIR}/include"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /W4)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_link_options(${PROJECT_NAME} PRIVATE /LARGEADDRESSAWARE)
endif()

