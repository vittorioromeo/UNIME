cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")
project(SmallProjects)

set(CMAKE_C_FLAGS "-std=c99 -lm -Wall -Wextra -W -pedantic -pthread" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -O3" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_DEBUG "-fno-omit-frame-pointer -g3 -gdwarf-2 -O0" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/;${CMAKE_MODULE_PATH}")

find_package(VeeLib REQUIRED)

set(SRC_DIR "src")
set(INC_DIR "include")
file(GLOB_RECURSE SRC_LIST "${INC_DIR}/*" "${SRC_DIR}/*")

include_directories("./include/")
include_directories(${VEELIB_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SRC_LIST})

install(TARGETS SmallProjects RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/_RELEASE/)
