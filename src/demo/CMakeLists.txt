cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

add_library(cmdargs INTERFACE)
target_include_directories(cmdargs INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/cmdargs
)

add_executable(vischio
    main.cpp
)

target_link_libraries(vischio
    vischio::lib
    cmdargs
)

if (NOT ${MVSC})
    target_compile_options(vischio
        PUBLIC
        -std=c++17
        PRIVATE
        -pendatic
        -Wall
        -Wextra
        -Werror
    )
endif (NOT ${MVSC})
