cmake_minimum_required(VERSION 3.22)
project(OpenGL_Learning)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL_Learning main.cpp glad/src/glad.c printers.h geometry.cpp geometry.h scene.cpp scene.h camera.cpp camera.h)
target_link_libraries(OpenGL_Learning glfw dl GL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
