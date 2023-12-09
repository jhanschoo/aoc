cmake_minimum_required(VERSION 3.20)

project(AOC CXX)

add_library(tutorial_compiler_flags INTERFACE)
find_package(Boost COMPONENTS chrono date_time REQUIRED)
find_package(range-v3 REQUIRED)
find_package(fmt REQUIRED)

include_directories(${BOOST_INCLUDE_DIRS})

target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_20)
target_compile_options(tutorial_compiler_flags INTERFACE "-Wall")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(aoc_default_libs tutorial_compiler_flags range-v3::range-v3 fmt::fmt ${Boost_LIBRARIES})

set(HARNESS_FILE "${CMAKE_BINARY_DIR}/harness.sh")
file(WRITE "${HARNESS_FILE}" "exec \"$1\" \"$2\" < \"$2\"\n")


include(CTest)

add_subdirectory(2022)
add_subdirectory(2023)
