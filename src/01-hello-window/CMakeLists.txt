cmake_minimum_required(VERSION 3.10)
project(hello-window)

include_directories("D:\\Documents\\OpenGL\\include")
LINK_DIRECTORIES("D:\\Documents\\OpenGL\\libs")
LINK_LIBRARIES(glad glfw3 opengl32)

set(CMAKE_CXX_STANDARD 14)

add_executable(hello-window main.cpp)

