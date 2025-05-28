if(NOT TARGET OpenGL::GL)
    find_library(OPENGL_gl_LIBRARY GL)
    find_path(OPENGL_INCLUDE_DIR GL/gl.h)
    if(OPENGL_gl_LIBRARY AND OPENGL_INCLUDE_DIR)
        add_library(OpenGL::GL UNKNOWN IMPORTED)
        set_target_properties(OpenGL::GL PROPERTIES
            IMPORTED_LOCATION "${OPENGL_gl_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${OPENGL_INCLUDE_DIR}"
        )
    else()
        message(FATAL_ERROR "Could not find OpenGL on Linux")
    endif()
endif()