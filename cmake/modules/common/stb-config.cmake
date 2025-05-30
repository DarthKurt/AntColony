if(NOT TARGET STB::TRUETYPE)
    FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG        6e9f34d5429cf16790ec43c9bac3f1ee4ad1f760
    )
    FetchContent_MakeAvailable(stb)

    set(STB_TRUETYPE_PATH "${stb_SOURCE_DIR}/stb_truetype.h")
    if(NOT EXISTS ${STB_TRUETYPE_PATH})
        message(FATAL_ERROR "Failed to install stb_truetype, file not found: ${STB_TRUETYPE_PATH}")
    else()
        add_library(stb INTERFACE)
        target_include_directories(stb INTERFACE 
            "${stb_SOURCE_DIR}"
        )
    endif()
endif()
