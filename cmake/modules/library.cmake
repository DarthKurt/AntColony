message("Configure lib task")
add_library(AntColonySimLib)

# Lib sources
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
file(GLOB_RECURSE RENDER_SOURCES "src/render/*.cpp")
file(GLOB_RECURSE SIMULATION_SOURCES "src/simulation/*.cpp")
file(GLOB_RECURSE UTILS_SOURCES "src/utils/*.cpp")

pretty_print_files("Core sources" CORE_SOURCES)
pretty_print_files("Render sources" RENDER_SOURCES)
pretty_print_files("Simulation sources" SIMULATION_SOURCES)
pretty_print_files("Utils sources" UTILS_SOURCES)

target_sources(
    AntColonySimLib
    PUBLIC
    ${CORE_SOURCES}
    ${RENDER_SOURCES}
    ${SIMULATION_SOURCES}
    ${UTILS_SOURCES}
)

target_link_libraries(
    AntColonySimLib
    PRIVATE
    glfw
    OpenGL::GL
)
message("Lib configuration done")