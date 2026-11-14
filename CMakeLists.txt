cmake_minimum_required(VERSION 3.20)

project(qpr2_recomp LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Windows 10/11 with the D3D11 native renderer is the performance target.
# Linux x86-64 builds the same title code against the runtime's POSIX layer:
# SDL2 window, software NV2A raster (the D3D11 ROV path is Windows-only), and
# the same FIFO executor, frame trace and report counters. It exists so the
# guest boot path and the tooling can be exercised without a Windows machine;
# its frame rate says nothing about the Windows renderer.
if(NOT WIN32 AND NOT (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64"))
    message(FATAL_ERROR
        "QPR2 clean v1 builds on Windows 10/11 and on Linux x86-64. "
        "The analysis/lift pipeline can still run on other hosts.")
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

# Where the locally generated code lives: <root>/recomp/gen/*.c and
# <root>/generated/qpr2_build_config.h. tools/qpr2_link_check.py points this
# at a stub tree to link the hand-written sources without a retail XBE.
set(QPR2_GENERATED_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE PATH
    "Root of the generated lift (recomp/gen) and build config (generated)")

set(XBOXRECOMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xboxrecomp"
    CACHE PATH "Path to the xboxrecomp toolkit")

if(NOT EXISTS "${XBOXRECOMP_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "XboxRecomp was not found at ${XBOXRECOMP_DIR}. Run: "
        "git submodule update --init --recursive")
endif()

file(GLOB RECOMP_GEN_SOURCES CONFIGURE_DEPENDS
    "${QPR2_GENERATED_ROOT}/recomp/gen/*.c")

if(NOT RECOMP_GEN_SOURCES)
    message(FATAL_ERROR
        "No recompiled QPR2 code was found. Put your own default.xbe in game/ "
        "and run: py -3 tools/qpr2_pipeline.py all")
endif()

if(NOT EXISTS "${QPR2_GENERATED_ROOT}/generated/qpr2_build_config.h")
    message(FATAL_ERROR
        "QPR2 build metadata is missing. Run: py -3 tools/qpr2_pipeline.py analyze")
endif()

# QPR2 drives the NV2A pushbuffer executor directly. On POSIX hosts the
# runtime's bundled D3D8-on-OpenGL layer is therefore not needed, which also
# drops the libepoxy/OpenGL build dependency. Windows keeps it: the D3D11
# presenter and renderer share its link libraries.
if(NOT WIN32)
    set(XBOXRECOMP_BUILD_D3D8 OFF CACHE BOOL "Build the bundled xbox_d3d8 compatibility layer" FORCE)
endif()

# XboxRecomp embeds GPU shaders at build time from the local sources.
add_subdirectory("${XBOXRECOMP_DIR}" "${CMAKE_BINARY_DIR}/xboxrecomp")


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
    src/qpr2_input_config.c
    src/qpr2_display.c
    ${RECOMP_GEN_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE xboxrecomp)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d11
        dxgi
        dxguid
        xinput
        winmm
        dbghelp
        user32
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${QPR2_GENERATED_ROOT}"
    "${QPR2_GENERATED_ROOT}/generated"
    "${QPR2_GENERATED_ROOT}/recomp"
    "${QPR2_GENERATED_ROOT}/recomp/gen"
    "${XBOXRECOMP_DIR}/include"
)
if(NOT WIN32)
    # The umbrella header still names the D3D8 interface header; only its
    # declarations are needed, not the OpenGL library behind it.
    target_include_directories(${PROJECT_NAME} PRIVATE "${XBOXRECOMP_DIR}/src/d3d")
endif()

option(QPR2_TRACK_GUEST_PC "Record lifted guest block addresses for debugging" OFF)
if(QPR2_TRACK_GUEST_PC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE RECOMP_ENABLE_GUEST_PC=1)
endif()

if(NOT MSVC)
    # Lifted code relies on wrapping integer arithmetic and type-punned guest
    # memory accesses.
    target_compile_options(${PROJECT_NAME} PRIVATE -O2 -g -fwrapv -fno-strict-aliasing)
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /W4)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/Zi>)
    target_link_options(${PROJECT_NAME} PRIVATE /LARGEADDRESSAWARE
        "$<$<CONFIG:Release>:/DEBUG:FULL;/OPT:REF;/OPT:ICF>")
endif()

