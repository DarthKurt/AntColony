# windows-toolchain.cmake
if(TOOLCHAIN_INITIALIZED)
    return()
endif()
set(TOOLCHAIN_INITIALIZED TRUE)

message(STATUS "Start of toolchain init")
message(STATUS "CMAKE_MODULES_DIR: ${CMAKE_MODULES_DIR}")
message(STATUS "TARGET_PLATFORM: ${TARGET_PLATFORM}")
message(STATUS "Using compiler toolchain file: ${LLVM_TOOLCHAIN_FILE}")

# Pass toolchain variables to try_compile
set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
    LLVM_TOOLCHAIN_FILE
)

# Disable PowerShell usage in vcpkg
set(VCPKG_POWERSHELL_PATH "")

if(LLVM_TOOLCHAIN_FILE)
    include("${LLVM_TOOLCHAIN_FILE}")
else()
    message(FATAL_ERROR "LLVM_TOOLCHAIN_FILE is not set.")
endif()
include("/opt/vcpkg/scripts/buildsystems/vcpkg.cmake")