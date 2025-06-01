# Static analysis with cppcheck
find_program(CPP_CHECK NAMES cppcheck)
if(CPP_CHECK)
    add_custom_target(run_cppcheck
        COMMAND ${CPP_CHECK} --enable=all --suppress=missingIncludeSystem --inconclusive --std=c++17 --force
                "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/tests"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Running cppcheck static analysis"
    )
endif()


file(GLOB_RECURSE CPP_FILES src/*.cpp)
add_custom_target(
    "function-complexity"
    COMMAND clang-tidy  "-checks=-*,readability-function-*" -p ${CMAKE_BINARY_DIR} ${CPP_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
