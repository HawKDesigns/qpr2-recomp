# Cross toolchain for tools/qpr2_link_check.py --toolchain: compiles and links
# the Windows configuration (D3D11 renderer, Win32 presenter, VEH handler) on
# a Linux host with MinGW-w64. It checks that the Windows code paths build;
# the supported Windows compiler is still MSVC.
#   apt install gcc-mingw-w64-x86-64-win32 g++-mingw-w64-x86-64-win32
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
