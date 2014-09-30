# Author: Matt Langston
# Date: 2014.09.20
cmake_minimum_required(VERSION 3.0.0)
project(TiCoreTests)

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

# Define helper functions and macros.
include(${PROJECT_SOURCE_DIR}/cmake/internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Allow "make test" to work.
enable_testing()

find_package(JavaScriptCore REQUIRED)
add_definitions(-DSTATICALLY_LINKED_WITH_JavaScriptCore)

include_directories(${JavaScriptCore_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(test)
