set(source_dirs 1.1 1.2 2.1 2.2 3.1 3.2 4.1 4.2 5.1 5.2 6.1 6.2 8.1 8.2
        #             9.1 9.2 10.1 10.2 12.1 25.1
        )

foreach (source_dir IN LISTS source_dirs)
    add_executable(AOC2022_${source_dir} "${source_dir}/main.cxx")
    target_link_libraries(AOC2022_${source_dir} PUBLIC ${aoc_default_libs})

    file(GLOB diagnostics "${source_dir}/diagnostic*.txt")
    set(diagnostic_index 1)
    foreach (diagnostic IN LISTS diagnostics)
        add_test(NAME "AOC2022_${source_dir}_diagnostic_${diagnostic_index}"
                COMMAND sh
                "${HARNESS_FILE}"
                "$<TARGET_FILE:AOC2022_${source_dir}>" "${diagnostic}"
                )
        math(EXPR diagnostic_index "${diagnostic_index} + 1")
    endforeach ()

    add_test(NAME AOC2022_${source_dir}_test
            COMMAND sh
            "${HARNESS_FILE}" "$<TARGET_FILE:AOC2022_${source_dir}>"
            "${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/input.txt"
            )
endforeach ()
