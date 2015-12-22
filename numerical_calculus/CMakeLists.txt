cmake_minimum_required(VERSION 3.0)

# Include SSVCmake.
list(APPEND CMAKE_MODULE_PATH
    "/home/vittorioromeo/OHWorkspace/SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/../SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/extlibs/SSVCMake/cmake/modules/")

include(SSVCMake)

# Basic project setup.
vrm_cmake_init_project(nc_cpp)
# vrm_cmake_find_extlib(vrm_pp)
# vrm_cmake_find_extlib(vrm_core)

# Alias the include directory.
set(NC_CPP_INC_DIR "${NC_CPP_SOURCE_DIR}/include/")

# Other compiler flags.
vrm_cmake_add_common_compiler_flags()

# Setup user options.
vrm_cmake_add_option_memcheck()
vrm_cmake_add_option_no_exceptions()
vrm_cmake_add_option_werror()

# The `check` target runs all tests and examples.
vrm_check_target()

# Setup subdirectories.
add_subdirectory(test)

# Create header-only install target (automatically glob)
vrm_cmake_header_only_install_glob("${NC_CPP_INC_DIR}" "include")
