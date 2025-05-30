if(NOT TARGET glad)
    add_library(glad
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/src/glad.c
    )
    set_target_properties(glad PROPERTIES
        LINKER_LANGUAGE C
        INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
    )
endif()