cmake_minimum_required(VERSION 3.10)
project(AntColonySim)

set(CMAKE_CXX_STANDARD 11)

# Добавляем исходники
file(GLOB SIMULATION_SOURCES "src/*.[c,h]pp")
add_executable(AntColonySim "src/main.cpp" ${SIMULATION_SOURCES})

# Находим GLFW через pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Создаем локальную директорию `libs` и копируем GLFW
set(LIBS_DIR "${CMAKE_BINARY_DIR}/libs")

file(MAKE_DIRECTORY ${LIBS_DIR})

add_custom_target(copy_glfw ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${GLFW_LIBRARY_DIRS}/libglfw.dylib" "${LIBS_DIR}/libglfw.dylib"
)

add_dependencies(AntColonySim copy_glfw)

# Используем относительный путь для подключения библиотеки
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${LIBS_DIR})

# Находим OpenGL
find_package(OpenGL REQUIRED)

# Подключаем зависимости по платформе
if(APPLE)
    target_compile_definitions(AntColonySim PRIVATE -DGL_SILENCE_DEPRECATION)
    target_link_libraries(AntColonySim "${LIBS_DIR}/libglfw.dylib" ${OPENGL_LIBRARIES}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo")
elseif(WIN32)
    target_link_libraries(AntColonySim ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} opengl32 gdi32)
elseif(UNIX)
    target_link_libraries(AntColonySim ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} dl X11 pthread)
endif()
