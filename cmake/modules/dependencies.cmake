find_package(OpenGL CONFIG REQUIRED)
message(STATUS "OpenGL lib found")

find_package(glfw3 CONFIG REQUIRED)
message(STATUS "GLFW found")

find_package(freetype CONFIG REQUIRED)
message(STATUS "FreeType2 found")

find_package(Catch2 CONFIG REQUIRED)
message(STATUS "Catch2 found")