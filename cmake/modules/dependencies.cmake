# Define the reusable macro
macro(add_dependency target package shortname)
    find_package("${package}" CONFIG REQUIRED)
    message(STATUS "${target} dependency configured")

    # Debug target settings
    get_target_property(${shortname}_type ${target} TYPE)
    message(STATUS "Library target '${target}' TYPE: ${${shortname}_type}")

    get_target_property(${shortname}_iic ${target} INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "Library target '${target}' INTERFACE_INCLUDE_DIRECTORIES: ${${shortname}_iic}")

    get_target_property(${shortname}_ic ${target} INCLUDE_DIRECTORIES)
    message(STATUS "Library target '${target}' INCLUDE_DIRECTORIES: ${${shortname}_ic}")

    get_target_property(${shortname}_imports ${target} IMPORTED_LOCATION)
    message(STATUS "Library target '${target}' IMPORTED_LOCATION: ${${shortname}_imports}")

    get_target_property(${shortname}_icfg ${target} IMPORTED_CONFIGURATIONS)
    message(STATUS "Library target '${target}' IMPORTED_CONFIGURATIONS: ${${shortname}_icfg}")

    get_target_property(${shortname}_link_libs ${target} INTERFACE_LINK_LIBRARIES)
    message(STATUS "Library target '${target}' INTERFACE_LINK_LIBRARIES: ${${shortname}_link_libs}")
    
    get_target_property(${shortname}_compile_defs ${target} INTERFACE_COMPILE_DEFINITIONS)
    message(STATUS "Library target '${target}' INTERFACE_COMPILE_DEFINITIONS: ${${shortname}_compile_defs}")
    
    get_target_property(${shortname}_compile_opts ${target} INTERFACE_COMPILE_OPTIONS)
    message(STATUS "Library target '${target}' INTERFACE_COMPILE_OPTIONS: ${${shortname}_compile_opts}")
    
    get_target_property(${shortname}_version ${target} VERSION)
    message(STATUS "Library target '${target}' VERSION: ${${shortname}_version}")
endmacro()

# set(CMAKE_FIND_DEBUG_MODE 1)
add_dependency("OpenGL::GL" "OpenGL" "gl")
add_dependency("glfw" "glfw3" "glfw")
add_dependency("glad" "glad" "glad")
add_dependency("stb" "stb" "stb")
add_dependency("dejavu" "dejavu-fonts" "dejavu")
add_dependency("glm" "glm" "glm")
# add_dependency("Catch2::Catch2WithMain" "Catch2" "Catch2")
