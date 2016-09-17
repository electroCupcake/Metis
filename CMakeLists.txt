cmake_minimum_required(VERSION 3.6.1)

#include directory, same for all build types
include_directories(include)

#file GLOB matches files and stores them in the specified variable, in this case, SOURCES
file(GLOB SOURCES "src/*.c")

if(FAB STREQUAL "po")
        message(STATUS "CRAP")
else()
        message(STATUS "NO CRAP")
endif()
