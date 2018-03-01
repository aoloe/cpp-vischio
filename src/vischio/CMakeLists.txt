cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

add_library(vischio-lib INTERFACE
)

add_library(vischio::lib ALIAS vischio-lib)

target_include_directories(vischio-lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

if (NOT ${MVSC})
    target_compile_options(vischio-lib
        PUBLIC
        -std=c++17
        PRIVATE
        -pendatic
        -Wall
        -Wextra
        -Werror
    )
endif (NOT ${MVSC})
