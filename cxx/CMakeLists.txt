cmake_minimum_required(VERSION 3.20)

project(AOC CXX)

add_library(tutorial_compiler_flags INTERFACE)

target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_20)
target_compile_options(tutorial_compiler_flags INTERFACE "-Wall")

add_subdirectory(2022)
