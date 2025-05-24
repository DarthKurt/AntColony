cmake_minimum_required(VERSION 3.16)
project(AntColonySim)

set(CMAKE_CXX_STANDARD 17)

find_program(CPP_CHECK NAMES cppcheck)
if(CPP_CHECK)
    add_custom_target(run_cppcheck
        COMMAND ${CPP_CHECK} --enable=all --check-level=exhaustive --suppress=missingIncludeSystem --inconclusive --std=c++17 --force "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/tests"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

# Add sources
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
file(GLOB_RECURSE RENDER_SOURCES "src/render/*.cpp")
file(GLOB_RECURSE SIMULATION_SOURCES "src/simulation/*.cpp")
file(GLOB_RECURSE UTILS_SOURCES "src/utils/*.cpp")
add_executable(AntColonySim "${CORE_SOURCES}" "${RENDER_SOURCES}" "${SIMULATION_SOURCES}" "${UTILS_SOURCES}" "src/main.cpp")

# Find & Copy GLFW
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
set(LIBS_DIR "${CMAKE_BINARY_DIR}/libs")
file(MAKE_DIRECTORY ${LIBS_DIR})

if(APPLE)
    add_custom_target(copy_glfw ALL
        COMMAND ${CMAKE_COMMAND} -E copy "${GLFW_LIBRARY_DIRS}/libglfw.dylib" "${LIBS_DIR}/libglfw.dylib"
    )
elseif(MINGW)
    add_custom_target(copy_glfw ALL
        COMMAND ${CMAKE_COMMAND} -E copy "${GLFW_LIBRARY_DIRS}/libglfw3.a" "${LIBS_DIR}/libglfw3.a"
    )
endif()

add_dependencies(AntColonySim copy_glfw)

# Include
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${LIBS_DIR})

# Look for OpenGL
find_package(OpenGL REQUIRED)

# Platform-specific builds
if(APPLE)
    target_compile_definitions(AntColonySim PRIVATE -DGL_SILENCE_DEPRECATION)
    target_link_libraries(AntColonySim 
        "${LIBS_DIR}/libglfw.dylib"
        "${OPENGL_LIBRARIES}")
elseif(WIN32)
    target_link_libraries(AntColonySim ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} opengl32 gdi32)
elseif(UNIX)
    target_link_libraries(AntColonySim ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} dl X11 pthread)
elseif(MINGW)
    target_link_libraries(AntColonySim 
        "${OPENGL_LIBRARIES}"
        gdi32
        opengl32
        "${LIBS_DIR}/libglfw3.a")
endif()

# Add tests
if(APPLE)
    enable_testing()
    find_package(Catch2 REQUIRED)
    file(GLOB_RECURSE SIMULATION_TEST_SOURCES "tests/simulation/*.cpp")
    add_executable(AntColonySimTests "${SIMULATION_TEST_SOURCES}" "${CORE_SOURCES}" "${RENDER_SOURCES}" "${SIMULATION_SOURCES}" "${UTILS_SOURCES}")
    target_compile_definitions(AntColonySimTests PRIVATE -DGL_SILENCE_DEPRECATION)
    target_link_libraries(AntColonySimTests "${LIBS_DIR}/libglfw.dylib" "${OPENGL_LIBRARIES}" "Catch2::Catch2WithMain")
    include(Catch)
    catch_discover_tests(AntColonySimTests)
endif()
