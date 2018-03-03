# Find the Catch test framework or download a single header file.
#
# This is not an official module.
#
# It assumes that Catch is not REQUIRED and that a minimum
# version is provided (not EXACT).
# If a suitable version isn't found locally, the single header
# file might be downloaded and placed in the Catch_ROOT directory
#
# This code read the following variables:
#  Catch_ROOT
#  Catch_DOWNLOAD
#
# This code sets the following variables:
#  Catch_FOUND            - Catch Testing framework found
#  Catch_INCLUDE_DIR      - path to catch.hpp
#  Catch_VERSION          - version number
#
#  Insipiration from:
#  - cpp-ug-luzern
#  - https://github.com/pybind/pybind11/blob/master/tools/FindCatch.cmake

if(NOT Catch_FIND_VERSION)
    message(FATAL_ERROR "A version number must be specified.")
elseif(Catch_FIND_REQUIRED)
    message(FATAL_ERROR "This module assumes Catch is not required.")
elseif(Catch_FIND_VERSION_EXACT)
    message(FATAL_ERROR "Exact version numbers are not supported, only minimum.")
endif()

# Extract the version number from catch.hpp
function(_get_catch_version)
    file(STRINGS "${Catch_INCLUDE_DIR}/catch.hpp" version_line REGEX "Catch v.*" LIMIT_COUNT 1)
    if(version_line MATCHES "Catch v([0-9]+)\\.([0-9]+)\\.([0-9]+)")
        set(Catch_VERSION "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}" PARENT_SCOPE)
    endif()
endfunction()

# Download the single-header version of Catch
function(_download_catch version destination_dir)
    message(STATUS "Downloading catch v${version}...")
    set(url https://github.com/philsquared/Catch/releases/download/v${version}/catch.hpp)
    file(DOWNLOAD ${url} "${destination_dir}/catch.hpp" STATUS status)
    list(GET status 0 error)
    if(error)
        message(FATAL_ERROR "Could not download ${url}")
    endif()
endfunction()

# Look for catch locally
find_path(Catch_INCLUDE_DIR NAMES
    catch.hpp
    HINTS
    $ENV{Catch_ROOT}
    ${Catch_ROOT}
    $ENV{Catch_ROOT}/single_include
    ${Catch_ROOT}/single_include
    $ENV{Catch_ROOT}/include
    ${Catch_ROOT}/include
    PATH_SUFFIXES catch)

if(Catch_INCLUDE_DIR)
    _get_catch_version()
endif()

# Download the header if it wasn't found or if it's outdated
message(STATUS Catch_VERSION)
if(NOT Catch_VERSION OR Catch_VERSION VERSION_LESS ${Catch_FIND_VERSION})
    if(Catch_DOWNLOAD)
        if(Catch_ROOT)
            _download_catch(${Catch_FIND_VERSION} "${Catch_ROOT}")
            _get_catch_version()
            if(Catch_VERSION)
                set(Catch_INCLUDE_DIR "${Catch_ROOT}" CACHE INTERNAL "")
            endif()
        else()
            message(STATUS "set(Catch_ROOT ...) is required to download Catch`.")
        endif()
    else()
        message(STATUS "`cmake -DDOWNLOAD_CATCH=1` or `set(Catch_DOWNLOAD TRUE)`"
            " to fetch Catch headers automatically.")
        # set(Catch_FOUND FALSE)
        return()
    endif()
endif()

if(NOT TARGET Catch::catch)
    add_library(Catch::catch INTERFACE IMPORTED)
    add_dependencies(Catch::catch catch)
    if(EXISTS "${Catch_INCLUDE_DIR}")
        set_target_properties(Catch::catch PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${Catch_INCLUDE_DIR}"
            )
    endif()
    # TODO: how to add target properties without removing the project ones?
    # set_target_properties(Catch::catch PROPERTIES
    #     INTERFACE_COMPILE_FEATURES cxx_noexcept
    #     INTERFACE_COMPILE_FEATURES cxx_std_11
    #     )
endif()

# set(Catch_FOUND TRUE)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Catch DEFAULT_MSG Catch_INCLUDE_DIR)
