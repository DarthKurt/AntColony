# windows-toolchain.cmake
if(TOOLCHAIN_INITIALIZED)
    return()
endif()
set(TOOLCHAIN_INITIALIZED TRUE)

message(STATUS "Start of toolchain init")
message(STATUS "CMAKE_MODULES_DIR: ${CMAKE_MODULES_DIR}")
message(STATUS "TARGET_PLATFORM: ${TARGET_PLATFORM}")
