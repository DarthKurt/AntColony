cmake_minimum_required(VERSION 4.0.2 FATAL_ERROR)

# Include core functions
include(cmake/modules/core.cmake)
include(FetchContent)

# Auto-detect if not provided
if(NOT TARGET_PLATFORM)
    execute_process(
        COMMAND uname -m
        OUTPUT_VARIABLE ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    if(ARCH STREQUAL "x86_64")
        set(TARGET_PLATFORM "linux-x64")
    elseif(ARCH STREQUAL "aarch64" OR ARCH STREQUAL "arm64")
        set(TARGET_PLATFORM "linux-arm64")
    endif()
endif()

# Your existing check
if(NOT TARGET_PLATFORM)
    message(FATAL_ERROR "TARGET_PLATFORM not set.")
endif()
message(STATUS "Target platform: ${TARGET_PLATFORM}")

# Configure osxcross toolchain if targeting macOS
if(TARGET_PLATFORM MATCHES "macOS")
    include(cmake/modules/osxcross-setup.cmake)
elseif(TARGET_PLATFORM MATCHES "linux")
    include(cmake/modules/linux-setup.cmake)
elseif(TARGET_PLATFORM MATCHES "windows")
    include(cmake/modules/windows-setup.cmake)
else()
    message(FATAL_ERROR "Unknown TARGET_PLATFORM '${TARGET_PLATFORM}'.")
endif()

project(AntColonySim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set custom modules and libs in priority

list(APPEND CMAKE_PREFIX_PATH_SWAP ${CMAKE_PREFIX_PATH})
list(REMOVE_DUPLICATES CMAKE_PREFIX_PATH_SWAP)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH_SWAP})

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

# Include dependencies
include(cmake/modules/dependencies.cmake)

# Sanitizer config
include(cmake/modules/sanitizers.cmake)

# Include build modules
include(cmake/modules/static-analysis.cmake)
include(cmake/modules/library.cmake)
include(cmake/modules/tests.cmake)

include(cmake/modules/main.cmake)
add_sanitizers_to_target(AntColonySim ${TARGET_PLATFORM})
