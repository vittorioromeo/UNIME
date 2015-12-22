# Add a custom target for the unit tests.
add_custom_target(tests COMMENT "Build all the unit tests.")

# `check` depends on `tests`.
add_dependencies(check tests)

# TODO:
# Override `add_executable`
macro (add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_target} -larmadillo -llapack -lblas)
    endif()
endmacro()

# Include directories.
include_directories(${VRM_CORE_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Generate all the header unit tests.
vrm_cmake_generate_public_header_tests_glob("*.hpp" "${VRM_CORE_SOURCE_DIR}/include")

# Generate all the unit tests.
vrm_cmake_generate_unit_tests_glob("*.cpp")
