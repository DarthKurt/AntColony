if(NOT TARGET OpenGL::GL)
    add_library(OpenGL::GL INTERFACE IMPORTED)
    set_target_properties(OpenGL::GL PROPERTIES
        INTERFACE_LINK_LIBRARIES "-lopengl32;-lgdi32;-lwinmm;-lunwind;"
        INTERFACE_INCLUDE_DIRECTORIES "${LLVM_MINGW_PATH}/${LLVM_MINGW_PREFIX}/include/GL"
    )
    message(STATUS "Configured OpenGL::GL with MinGW frameworks for Windows (${LLVM_MINGW_PREFIX})")
endif()
