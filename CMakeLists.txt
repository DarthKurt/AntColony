cmake_minimum_required(VERSION 4.0.2)

# Define the reusable function
function(pretty_print_files label list_ref)
    message(STATUS "${label}:")
    foreach(item IN LISTS ${list_ref})
        message("  ${item}")
    endforeach()
endfunction()

# Auto-detect if not provided
if(NOT TARGET_PLATFORM)
    execute_process(
        COMMAND uname -m
        OUTPUT_VARIABLE ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    if(ARCH STREQUAL "x86_64")
        set(TARGET_PLATFORM "linux-x64" CACHE STRING "Target platform" FORCE)
    elseif(ARCH STREQUAL "aarch64" OR ARCH STREQUAL "arm64")
        set(TARGET_PLATFORM "linux-arm64" CACHE STRING "Target platform" FORCE)
    endif()
endif()

# Your existing check
if(NOT TARGET_PLATFORM)
    message(FATAL_ERROR "TARGET_PLATFORM not set.")
endif()
message(STATUS "Target platform: ${TARGET_PLATFORM}")

# Map platforms to vcpkg triplets
set(PLATFORM_TO_TRIPLET_windows-x64 "x64-windows")
set(PLATFORM_TO_TRIPLET_windows-arm64 "arm64-windows")
set(PLATFORM_TO_TRIPLET_linux-x64 "x64-linux")
set(PLATFORM_TO_TRIPLET_linux-arm64 "arm64-linux")
set(PLATFORM_TO_TRIPLET_macOS-x64 "x64-osx")
set(PLATFORM_TO_TRIPLET_macOS-arm64 "arm64-osx")

# Auto-configure vcpkg triplet based on platform
if(TARGET_PLATFORM)
    set(TRIPLET_VAR_NAME "PLATFORM_TO_TRIPLET_${TARGET_PLATFORM}")
    if(DEFINED ${TRIPLET_VAR_NAME})
        set(VCPKG_TARGET_TRIPLET "${${TRIPLET_VAR_NAME}}" CACHE STRING "Vcpkg target triplet")
        message(STATUS "Auto-configured VCPKG_TARGET_TRIPLET to ${VCPKG_TARGET_TRIPLET} based on TARGET_PLATFORM")
    else()
        message(FATAL_ERROR "Unknown TARGET_PLATFORM '${TARGET_PLATFORM}'. VCPKG_TARGET_TRIPLET not auto-configured.")
    endif()
endif()

# Set vcpkg toolchain file (for automatic package handling)
set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(AntColonySim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

## OpenGL
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL lib found: ${OPENGL_LIBRARIES}")

# GLFW
find_package(glfw3 CONFIG REQUIRED)
message(STATUS "GLFW found")

# Catch2
find_package(Catch2 CONFIG REQUIRED)
message(STATUS "Catch2 found")

# Static analysis with cppcheck
find_program(CPP_CHECK NAMES cppcheck)
if(CPP_CHECK)
    add_custom_target(run_cppcheck
        COMMAND ${CPP_CHECK} --enable=all --suppress=missingIncludeSystem --inconclusive --std=c++17 --force
                "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/tests"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Running cppcheck static analysis"
    )
endif()

# Lib
message("Configure lib task")
add_library(AntColonySimLib)

# Lib sources
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
file(GLOB_RECURSE RENDER_SOURCES "src/render/*.cpp")
file(GLOB_RECURSE SIMULATION_SOURCES "src/simulation/*.cpp")
file(GLOB_RECURSE UTILS_SOURCES "src/utils/*.cpp")

pretty_print_files("Core sources" CORE_SOURCES)
pretty_print_files("Render sources" RENDER_SOURCES)
pretty_print_files("Simulation sources" SIMULATION_SOURCES)
pretty_print_files("Utils sources" UTILS_SOURCES)

target_sources(AntColonySimLib PUBLIC 
    ${CORE_SOURCES} 
    ${RENDER_SOURCES}
    ${SIMULATION_SOURCES}
    ${UTILS_SOURCES})

# Lib sources
target_link_libraries(AntColonySimLib PRIVATE "glfw" "OpenGL::GL")
message("Lib configuration done")

# Tests
message("Configure test task")
enable_testing()
include(Catch)

file(GLOB_RECURSE SIMULATION_TEST_SOURCES "tests/*.cpp")
pretty_print_files("Test sources" SIMULATION_TEST_SOURCES)
if(NOT SIMULATION_TEST_SOURCES)
    message(WARNING "No test source files found in tests/simulation/")
endif()
add_executable(AntColonySimTests "${SIMULATION_TEST_SOURCES}")
target_link_libraries(AntColonySimTests PRIVATE AntColonySimLib "glfw" "OpenGL::GL" "Catch2::Catch2WithMain")

catch_discover_tests(AntColonySimTests)
message("Test configuration done")
# Main
message("Configure executable task")
add_executable(AntColonySim "src/main.cpp")
target_link_libraries(AntColonySim PRIVATE AntColonySimLib glfw)
