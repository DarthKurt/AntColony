message("Configure executable task")
add_executable(AntColonySim "src/main.cpp")
target_link_libraries(
    AntColonySim
    PRIVATE
    AntColonySimLib
    glfw
    glad
)