cmake_minimum_required(VERSION 2.8)

project(position_based_fluids)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(HEADER
    "include"
    "lib/glad/include"
    "lib/stb_image/include"
    "lib/glm-0.9.8.4/include")
set(SOURCES
    "src/main.cpp"
    "src/application.cpp"
    "src/particleSystem.cpp")

include_directories(${HEADER})
include_directories(${OPENGL_INCLUDE_DIR})

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

set(GLAD "lib/glad/src/glad.c")

add_executable(${PROJECT_NAME} ${SOURCES} ${GLAD})

target_link_libraries(${PROJECT_NAME} glfw OpenGL)
