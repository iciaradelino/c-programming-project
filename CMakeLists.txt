cmake_minimum_required(VERSION 3.10)
project(BlackHoleSim C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -O2")

include_directories(include)
file(GLOB SOURCES "src/*.c")

# Find and link OpenGL + GLFW + GLEW
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

add_executable(blackhole ${SOURCES})
target_link_libraries(blackhole
    m
    pthread
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    GLEW
    GL
)
