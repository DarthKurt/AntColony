if(NOT TARGET glfw)

    set(GLFW_BUILD_WAYLAND "OFF")
    # Download GLFW release source from GitHub
    FetchContent_Declare(
        glfw-static
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        7b6aead9fb88b3623e3b3725ebb42670cbe4c579
    )
    # Ensure GLFW is downloaded and unpacked
    FetchContent_MakeAvailable(glfw-static)
endif()