INCLUDE(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)

set(PSX BOOL ON)
# specify the cross compiler
CMAKE_FORCE_C_COMPILER(CCPSX.EXE GNU)
CMAKE_FORCE_CXX_COMPILER(CCPSX.EXE GNU)

#set(INITRODE_CROSSCOMPILE_TOOLCHAIN ${_path_to_toolchain} CACHE PATH "Path to the cross compiler toolchain")
#set(CMAKE_FIND_ROOT_PATH ${INITRODE_CROSSCOMPILE_TOOLCHAIN})
# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)