cmake_minimum_required(VERSION 3.15)
project(CSVPrettyPrinter VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(csv_pretty_printer
    main.c
    csv_parser.c
    table_renderer.c
)
