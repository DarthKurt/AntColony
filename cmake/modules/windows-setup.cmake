message(STATUS "Configuring for Windows platform")

if(TARGET_PLATFORM STREQUAL "windows-x64")
    set(WINDOWS_TARGET_ARCH "x86_64")
    set(VCPKG_TARGET_ARCHITECTURE "64")
elseif(TARGET_PLATFORM STREQUAL "windows-arm64")
    set(WINDOWS_TARGET_ARCH "arm64")
    set(VCPKG_TARGET_ARCHITECTURE "arm64")
else()
    message(FATAL_ERROR "Unknown Windows TARGET_PLATFORM '${TARGET_PLATFORM}'.")
endif()

# Set vcpkg toolchain file (for automatic package handling)
set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_BINARY_DIR}/vcpkg_triplets")
file(MAKE_DIRECTORY "${VCPKG_OVERLAY_TRIPLETS}")

set(CMAKE_MODULES_DIR "${CMAKE_SOURCE_DIR}/cmake/modules/windows")
set(VCPKG_TARGET_TRIPLET "${TARGET_PLATFORM}-triplet")
set(LLVM_TOOLCHAIN_FILE "${CMAKE_MODULES_DIR}/${TARGET_PLATFORM}-llvm.cmake")

# Generate triplet file dynamically
file(WRITE "${VCPKG_OVERLAY_TRIPLETS}/${VCPKG_TARGET_TRIPLET}.cmake"
    "set(VCPKG_TARGET_ARCHITECTURE ${VCPKG_TARGET_ARCHITECTURE})\n"
    "set(VCPKG_CRT_LINKAGE static)\n"
    "set(VCPKG_LIBRARY_LINKAGE static)\n"
    "set(VCPKG_CMAKE_SYSTEM_NAME Windows)\n"
    "set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${LLVM_TOOLCHAIN_FILE})\n"
)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_MODULES_DIR}/windows-toolchain.cmake")
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# To give priority to the modules
set(CMAKE_PREFIX_PATH_SWAP "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_MODULES_DIR}/")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH_SWAP}")
