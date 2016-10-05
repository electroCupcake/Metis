cmake_minimum_required(VERSION 2.8)

#set global exe name
set(MAIN "movsqr")

project(${MAIN})
#include directory, same for all build types
include_directories(include)

#file GLOB matches files and stores them in the specified variable, in this case, SOURCES
#get main file for build
file(GLOB SOURCES "main.c")

#add global sources
file(GLOB GLB_SRC "src/*.c")
list(APPEND SOURCES ${GLB_SRC})

<<<<<<< HEAD
#set global exe name
set(MAIN "MAIN")

project(${MAIN})

=======
>>>>>>> fcb076a8dde0c231276c0d56707da96a3dfcc10d
#include sources that are platform specific
if(PSX)
        message(STATUS "PSX BUILD")
        
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_psx)
        
        file(GLOB PSX_SRC "src/psx/*.c")
        list(APPEND SOURCES ${PSX_SRC})
        
        add_executable(${MAIN}.cpe ${SOURCES})
        
        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MAIN}.exe)
        
        add_custom_command(TARGET ${MAIN}.cpe POST_BUILD COMMAND CPE2XWIN.EXE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MAIN}.cpe)
        
elseif(WIN32 OR MINGW OR MSYS OR CYGWIN OR MSVC)
        message(STATUS "WINDOWS BUILD")
        
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_win)
        
        file(GLOB WIN_SRC "src/win/*.c")
        list(APPEND SOURCES ${WIN_SRC})

        add_executable(${MAIN}.exe ${SOURCES})
elseif(APPLE OR WATCOM OR BORLAND)
        message(FATAL_ERROR "CAN NOT BUILD FOR APPLE, WATCOM, BORLAND")
elseif(UNIX)
        message(STATUS "LINUX BUILD")
        
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_nix)
        
        file(GLOB NIX_SRC "src/nix/*.c")
        list(APPEND SOURCES ${NIX_SRC})
        
        add_executable(${MAIN} ${SOURCES})
else()
        message(FATAL_ERROR "NO BUILD TARGET MATCH")
endif()

if(PSX)
        message(STATUS "LINKING PSX LIBS")
        target_compile_options(${MAIN}.cpe PUBLIC -O3 -Dpsx)
        target_link_libraries(${MAIN}.cpe libpad -Xo0x80010000)
else()
        message(STATUS "LINKING OPENGL, GLFW, GLEW LIBS")
        find_package(glfw3 3.2 REQUIRED)
        find_package(GLEW 1.10 REQUIRED)
        find_package(OpenGL REQUIRED)
        if(OPENGL_FOUND AND GLEW_FOUND AND glfw3_FOUND)
                if(UNIX)
                        message(STATUS "LINKING FOR LINUX")
                        target_link_libraries(${MAIN} ${OPENGL_LIBRARY} glfw GLEW)
                else()
                        message(STATUS "LINKING FOR WINDOWS")
                        target_link_libraries(${MAIN}.exe ${OPENGL_LIBRARY} glfw GLEW)
                endif()
        else()
                if(NOT OPENGL_FOUND)
                        message(FATAL_ERROR "NO OPENGL FOUND")
                endif()
                
                if(NOT GLEW_FOUND)
                        message(FATAL_ERROR "NO GLEW FOUND")
                endif()
                
                if(NOT glwf3_FOUND)
                        message(FATAL_ERROR "NO GLFW3 FOUND")
                endif()
        endif()
endif()
