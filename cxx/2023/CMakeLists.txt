set(source_dirs
        1.1 1.2
        2.1 2.2
        3.1 3.2
        4.1 4.2
        )

foreach (source_dir IN LISTS source_dirs)
    add_executable(AOC2023_${source_dir} "${source_dir}/main.cxx")
    target_link_libraries(AOC2023_${source_dir} PUBLIC ${aoc_default_libs})

    file(GLOB diagnostics "${source_dir}/diagnostic*.txt")
    set(diagnostic_index 1)
    foreach (diagnostic IN LISTS diagnostics)
        add_test(NAME "AOC2023_${source_dir}_diagnostic_${diagnostic_index}"
                COMMAND sh
                "${HARNESS_FILE}"
                "$<TARGET_FILE:AOC2023_${source_dir}>" "${diagnostic}"
                )
        math(EXPR diagnostic_index "${diagnostic_index} + 1")
    endforeach ()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/input.txt")
        add_test(NAME "AOC2023_${source_dir}_input"
                COMMAND sh
                "${HARNESS_FILE}"
                "$<TARGET_FILE:AOC2023_${source_dir}>"
                "${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/input.txt"
                )
    endif ()
endforeach ()
