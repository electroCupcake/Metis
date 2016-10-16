cmake_minimum_required(VERSION 2.8.4)

#set global exe name
set(MAIN "METIS")

project(${MAIN})
#include directory, same for all build types
include_directories(include)

#add global sources
file(GLOB SOURCES "src/*.c")

#include sources that are platform specific
if(PSX)
  message(STATUS "PSX BUILD")
  
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib_psx)
  
  file(GLOB ARCH_SRC "src/psx/*.c")
  
elseif(MINGW OR MSYS OR CYGWIN OR MSVC)
  message(STATUS "WINDOWS BUILD")
  
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib_win)
  
  file(GLOB ARCH_SRC "src/win/*.c")

elseif(APPLE OR WATCOM OR BORLAND)
  message(FATAL_ERROR "CAN NOT BUILD FOR APPLE, WATCOM, BORLAND")
  
elseif(UNIX)
  message(STATUS "LINUX BUILD")
  
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib_nix)
  
  file(GLOB ARCH_SRC "src/nix/*.c")
  
else()
  message(FATAL_ERROR "NO BUILD TARGET MATCH")
  
endif()

list(APPEND SOURCES ${ARCH_SRC})
add_library(${MAIN} STATIC ${SOURCES})
