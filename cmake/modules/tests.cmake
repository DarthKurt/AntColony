message("Configure test task")
enable_testing()
include(Catch)

file(GLOB_RECURSE SIMULATION_TEST_SOURCES "tests/*.cpp")
pretty_print_files("Test sources" SIMULATION_TEST_SOURCES)

if(NOT SIMULATION_TEST_SOURCES)
    message(WARNING "No test source files found in tests/simulation/")
endif()

add_executable(AntColonySimTests "${SIMULATION_TEST_SOURCES}")
target_link_libraries(
    AntColonySimTests
    PRIVATE
    AntColonySimLib
    glfw
    OpenGL::GL
    Catch2::Catch2WithMain
)

catch_discover_tests(AntColonySimTests)
message("Test configuration done")