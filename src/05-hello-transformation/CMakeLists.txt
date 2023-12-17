cmake_minimum_required(VERSION 3.10)
project(hello-transformation)

include_directories("./include")
include_directories("D:\\Documents\\OpenGL\\include")
LINK_DIRECTORIES("D:\\Documents\\OpenGL\\libs")
LINK_LIBRARIES(glad glfw3 opengl32)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
    src/main.cpp
    src/shader.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
