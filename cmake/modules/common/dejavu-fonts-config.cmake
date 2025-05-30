if(NOT TARGET dejavu)
    add_library(dejavu INTERFACE)
    target_include_directories(dejavu INTERFACE 
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/dejavu/"
    )
endif()