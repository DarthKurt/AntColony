if(NOT TARGET glfw)
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