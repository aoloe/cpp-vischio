cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

enable_testing(true)

# the top level directory has a a find module for catch
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
# set(CMAKE_MODULE_PATH ${CMAKE_PROJECT_SOURCE_DIR}/cmake)

set(Catch_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/catch)
find_package(Catch 2.1.2)

add_executable(unittest
   main.cpp
   emphasis.cpp
)

add_executable(vischio::unittest ALIAS unittest)

add_test(vischio
    vischio
)

target_link_libraries(unittest
    PRIVATE
    Catch::catch
    vischio::lib
)

# add_custom_target(run-unittest
#     ALL
#     vischio::unittest
# )

if (NOT ${MVSC})
    target_compile_options(unittest
        PUBLIC
        -std=c++17
        PRIVATE
        -pendatic
        -Wall
        -Wextra
        -Werror
    )
endif (NOT ${MVSC})
