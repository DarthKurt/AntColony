if(NOT TARGET glm)
    
    FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	    bf71a834948186f4097caa076cd2663c69a10e1e
        SOURCE_SUBDIR  cmake
    )

    FetchContent_MakeAvailable(glm)

    add_library(glm INTERFACE)
    target_include_directories(stb INTERFACE 
        "${glm_SOURCE_DIR}"
    )
    
endif()
