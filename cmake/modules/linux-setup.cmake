message(STATUS "Configuring for Linux platform")

if(TARGET_PLATFORM STREQUAL "linux-x64")
    set(WINDOWS_TARGET_ARCH "x86_64")
    set(VCPKG_TARGET_ARCHITECTURE "64")
elseif(TARGET_PLATFORM STREQUAL "linux-arm64")
    set(WINDOWS_TARGET_ARCH "arm64")
    set(VCPKG_TARGET_ARCHITECTURE "arm64")
else()
    message(FATAL_ERROR "Unknown Linux TARGET_PLATFORM '${TARGET_PLATFORM}'.")
endif()

set(CMAKE_MODULES_DIR "${CMAKE_SOURCE_DIR}/cmake/modules/linux")

# Set vcpkg toolchain file (for automatic package handling)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_MODULES_DIR}/linux-toolchain.cmake")
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/common/" "${CMAKE_MODULES_DIR}/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH_SWAP "${CMAKE_PREFIX_PATH}")

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_PREFIX_PATH_SWAP: ${CMAKE_PREFIX_PATH_SWAP}")
