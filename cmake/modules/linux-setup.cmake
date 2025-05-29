message(STATUS "Configuring for Linux platform")

# Set vcpkg toolchain file (for automatic package handling)
set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake")
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# To give priority to the modules
set(CMAKE_PREFIX_PATH_SWAP "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/linux/")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH_SWAP}")