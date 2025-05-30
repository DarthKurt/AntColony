message(STATUS "Configuring for Linux platform")

set(CMAKE_MODULES_DIR "${CMAKE_SOURCE_DIR}/cmake/modules/linux")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_MODULES_DIR}/linux-toolchain.cmake")
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/common/" "${CMAKE_MODULES_DIR}/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH_SWAP "${CMAKE_PREFIX_PATH}")

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_PREFIX_PATH_SWAP: ${CMAKE_PREFIX_PATH_SWAP}")
