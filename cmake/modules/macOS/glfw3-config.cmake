if(NOT TARGET glfw)

    # Download GLFW release source from GitHub
    FetchContent_Declare(
        glfw-static
        URL      https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.MACOS.zip
    )
    # Ensure GLFW is downloaded and unpacked
    FetchContent_MakeAvailable(glfw-static)

    # Create an imported static library target called glfw.
    add_library(glfw STATIC IMPORTED)
    # Set the imported location to the prebuilt static library;
    # adjust the path to match your downloaded layout.
    set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${glfw-static_SOURCE_DIR}/lib-${OSXCROSS_TARGET_ARCH}/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${glfw-static_SOURCE_DIR}/include"
    )
    message(STATUS "Configured imported static target 'glfw' with includes at ${glfw-static_SOURCE_DIR}/include")
endif()