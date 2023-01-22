set(source_dirs 1.1 1.2 2.1 2.2 3.1 3.2 4.1 4.2 5.1 5.2
        6.1 6.2 7.1 7.2 8.1 8.2 9.1 9.2 10.1 10.2
        11.1 11.2 12.1 12.2 13.1 13.2 14.1 14.2 15.1 15.2
#        16.1 16.2 17.1 17.2 18.1 18.2 20.1 20.2
        21.1 21.2 22.1
        25.1)

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

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/input.txt")
        add_test(NAME "AOC2022_${source_dir}_input"
                COMMAND sh
                "${HARNESS_FILE}"
                "$<TARGET_FILE:AOC2022_${source_dir}>"
                "${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/input.txt"
                )
    endif ()
endforeach ()
