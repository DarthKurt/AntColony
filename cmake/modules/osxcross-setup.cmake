if(TARGET_PLATFORM STREQUAL "macOS-x64")
    set(OSXCROSS_TARGET_ARCH "x86_64")
elseif(TARGET_PLATFORM STREQUAL "macOS-arm64")
    set(OSXCROSS_TARGET_ARCH "arm64")
else()
    message(FATAL_ERROR "Unknown MacOS TARGET_PLATFORM '${TARGET_PLATFORM}'.")
endif()

if(NOT EXISTS ${OSXCROSS_CCTOOLS_PATH})
    set(OSXCROSS_CCTOOLS_PATH "/sdks/osxcross/bin")
endif()

# Run osxcross-conf and capture its output.
execute_process(
    COMMAND "${OSXCROSS_CCTOOLS_PATH}/osxcross-conf"
    RESULT_VARIABLE OSXCROSS_CONF_RESULT
    OUTPUT_VARIABLE OSXCROSS_CONF_OUTPUT
    ERROR_VARIABLE OSXCROSS_CONF_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT OSXCROSS_CONF_RESULT EQUAL 0)
    message(FATAL_ERROR "osxcross-conf failed: ${OSXCROSS_CONF_ERROR}")
endif()

# Split output into lines.
string(REPLACE "\n" ";" OUTPUT_LINES "${OSXCROSS_CONF_OUTPUT}")
message(STATUS "Processing osxcross-conf: ${OSXCROSS_CONF_OUTPUT}")
foreach(line IN LISTS OUTPUT_LINES)
    if(line MATCHES "^export[ \t]*([A-Z0-9_]+)=(.*)$")
        set(${CMAKE_MATCH_1} ${CMAKE_MATCH_2})
        set(var_name "${CMAKE_MATCH_1}")
        set(var_value "${CMAKE_MATCH_2}")
        set(ENV{${var_name}} "${var_value}")
        message("${var_name} = ${var_value}")
    else()
        message("No match for line: ${line}")
    endif()
endforeach()

if(NOT OSXCROSS_TARGET)
    message(FATAL_ERROR "No darwin tools found in ${OSXCROSS_BIN_DIR}")
endif()

if(NOT OSXCROSS_OSX_VERSION_MIN)
    message(FATAL_ERROR "OSXCROSS_OSX_VERSION_MIN not found")
endif()

# GLFW has minimum 11.0.0
set(ENV{OSXCROSS_OSX_VERSION_MIN} "11.0.0")
set(ENV{MACOSX_DEPLOYMENT_TARGET} "11.0.0")
set(ENV{PATH} "${OSXCROSS_CCTOOLS_PATH}:$ENV{PATH}")

# Prepare MacPorts
set(OSXCROSS_MACPORTS "${OSXCROSS_CCTOOLS_PATH}/osxcross-macports")
if(NOT EXISTS ${OSXCROSS_MACPORTS})
    message(FATAL_ERROR "MacPorts not found at ${OSXCROSS_MACPORTS}")
endif()
set(OSXCROSS_MACPORTS_DIR "${OSXCROSS_TARGET_DIR}/macports")

# Clean install
file(REMOVE_RECURSE "${OSXCROSS_MACPORTS_DIR}")
file(MAKE_DIRECTORY "${OSXCROSS_MACPORTS_DIR}")
file(WRITE "${OSXCROSS_MACPORTS_DIR}/MIRROR" "https://packages.macports.org")

message("Installing MacPorts with ${OSXCROSS_MACPORTS} install catch2")
execute_process(
    COMMAND ${OSXCROSS_MACPORTS} install catch2
    RESULT_VARIABLE INSTALL_RESULT
    OUTPUT_VARIABLE INSTALL_OUTPUT
    ERROR_VARIABLE INSTALL_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT INSTALL_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to install MacPorts: ${INSTALL_ERROR} ${INSTALL_RESULT} ${INSTALL_OUTPUT}")
else()
    message(STATUS "Successfully installed MacPorts.")
endif()

# set(CMAKE_FIND_DEBUG_MODE ON)
set(OSXCROSS_FRAMEWORKS_DIR "${OSXCROSS_SDK}/System/Library/Frameworks")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/macOS/")
list(APPEND CMAKE_PREFIX_PATH "${OSXCROSS_MACPORTS_DIR}/pkgs/opt/local/lib/cmake/")
list(APPEND CMAKE_PREFIX_PATH "${OSXCROSS_MACPORTS_DIR}/pkgs/opt/local/share/cmake/")

include(FetchContent)

# Download GLFW release source from GitHub
FetchContent_Declare(
    glfw-static
    URL      https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.MACOS.zip
)
# Ensure GLFW is downloaded and unpacked
FetchContent_MakeAvailable(glfw-static)

set(TOOLCHAIN_PREFIX "${OSXCROSS_TARGET_ARCH}-apple-${OSXCROSS_TARGET}")
set(CMAKE_C_COMPILER "${OSXCROSS_CCTOOLS_PATH}/${TOOLCHAIN_PREFIX}-clang")
set(CMAKE_CXX_COMPILER "${OSXCROSS_CCTOOLS_PATH}/${TOOLCHAIN_PREFIX}-clang++")
set(CMAKE_C_COMPILER_AR "${OSXCROSS_CCTOOLS_PATH}/${TOOLCHAIN_PREFIX}-ar")
set(CMAKE_CXX_COMPILER_AR "${OSXCROSS_CCTOOLS_PATH}/${TOOLCHAIN_PREFIX}-ar")
set(CMAKE_LINKER "${OSXCROSS_CCTOOLS_PATH}/${TOOLCHAIN_PREFIX}-ld")

set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=${CMAKE_LINKER}")
set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=${CMAKE_LINKER}")
set(CMAKE_MODULE_LINKER_FLAGS "-fuse-ld=${CMAKE_LINKER}")
