cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(PROJECT_NAME "myWorld")
set(LIB_PATH "lib")
set(SRC_PATH "src")
set(INCLUDE_PATH "include")

project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wcomment -pedantic")
include_directories(SYSTEM ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} )
include_directories(SYSTEM ${INCLUDE_PATH})
file(GLOB_RECURSE LIB_SOURCES "${LIB_PATH}/*.cpp")
file(GLOB_RECURSE SOURCES "${SRC_PATH}/*.cpp")
file(GLOB_RECURSE PRIVATE_HEADERS "${SRC_PATH}/*.h")
file(GLOB_RECURSE SHADERS "${SRC_PATH}/*.glsl")
add_executable(${PROJECT_NAME} ${LIB_SOURCES} ${SOURCES} ${HEADERS} ${PRIVATE_HEADERS} ${SHADERS})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_STATIC_LIBRARIES})
