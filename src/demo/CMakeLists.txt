cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

add_compile_options(-std=c++17)

add_library(cmdargs INTERFACE)
target_include_directories(cmdargs INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/cmdargs
)

add_executable(vischio
    main.cpp
)

target_link_libraries(vischio
    cmdargs
    vischio::lib
)

if (NOT ${MVSC})
    target_compile_options(vischio
        PRIVATE
        -pendatic
        -Wall
        -Wextra
        -Werror
    )
endif (NOT ${MVSC})
