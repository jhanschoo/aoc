cmake_minimum_required(VERSION 3.20)

project(AOC CXX)

add_library(tutorial_compiler_flags INTERFACE)

target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_20)
target_compile_options(tutorial_compiler_flags INTERFACE "-Wall")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(aoc_default_libs tutorial_compiler_flags CONAN_PKG::range-v3)

set(HARNESS_FILE "${CMAKE_BINARY_DIR}/harness.sh")
file(WRITE "${HARNESS_FILE}" "exec $1 < $2\n")


include(CTest)


add_subdirectory(2022)
