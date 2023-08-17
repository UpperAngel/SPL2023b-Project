cmake_minimum_required(VERSION 3.25)
project(SPL2023b_Project_copy C)

set(CMAKE_C_STANDARD 90)

add_compile_options(-ansi -Wall -pedantic)

include_directories("src/pre-processor/pre-processor header files")

add_executable(SPL2023b_Project_copy
        "src/pre-processor/pre-processor header files/helper_fucntions.h"
        "src/pre-processor/pre-processor header files/pre_processor.h"
        "src/pre-processor/pre-processor header files/support_functions.h"
        src/pre-processor/main.c
        src/pre-processor/pre_processor.c
        src/pre-processor/support_functions.c
        src/create_file_func.c
        "src/FIRST ---PASS/david_first_pass.c"
        "src/FIRST ---PASS/Test.c"
        "src/FIRST ---PASS/support functions/functions.c"
        "src/FIRST ---PASS/support functions/line_foramting_functions.c"
        "src/FIRST ---PASS/header files/first_pass_headers.h"
        "src/FIRST ---PASS/support functions/handle_data_directive.c"
        "src/Error handling/handle_error.c"
        "src/Error handling/handle_error.h"
        "src/Error handling/errors.h")
