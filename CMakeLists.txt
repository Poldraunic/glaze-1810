cmake_minimum_required(VERSION 3.16)
project(Test)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Meta/CMake")
include(compile_options)

add_subdirectory(Libraries)
