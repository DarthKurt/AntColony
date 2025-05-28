# Sanitizer configuration module
option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(ENABLE_LSAN "Enable Leak Sanitizer" OFF)

# Helper function to add sanitizers to a target
function(add_sanitizers_to_target_core target)
    if(ENABLE_ASAN)
        target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address)
        message(STATUS "Address Sanitizer enabled for target: ${target}")
    endif()

    if(ENABLE_LSAN)
        # Standalone LSan for Linux
        if(NOT ENABLE_ASAN) # If ASan is enabled, it already includes leak detection
            target_compile_options(${target} PRIVATE -fsanitize=leak -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=leak)
            message(STATUS "Leak Sanitizer enabled for target: ${target}")
        endif()
    endif()
endfunction()

function(add_sanitizers_to_target target platform)
    if(ENABLE_ASAN OR ENABLE_LSAN)
        # Configure osxcross toolchain if targeting macOS
        if(${platform} MATCHES "linux")
            add_sanitizers_to_target_core(${target})
        else()
            message(FATAL_ERROR "Sanitizer for '${platform}' has not been implemented yet.")
        endif()
    endif()
endfunction()