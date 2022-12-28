set(source_dirs 1.1 1.2 2.1 2.2 3.1 3.2 4.1 4.2 5.1 5.2 6.1 6.2 8.1 8.2 9.1 9.2 10.1 10.2 12.1 25.1)

foreach(source_dir IN LISTS source_dirs)
    add_executable(AOC2022_${source_dir} "${source_dir}/main.cpp")
    target_link_libraries(AOC2022_${source_dir} PUBLIC tutorial_compiler_flags)
endforeach()
