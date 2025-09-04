cmake_minimum_required(VERSION 3.26.0)
project(opengl VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3)

# add_executable(opengl src/main.cpp src/glad.c src/Camera.cpp)
file(GLOB SRC_FILES src/*.cpp src/*.c)
add_executable(opengl ${SRC_FILES})

target_include_directories(opengl PRIVATE include)
target_include_directories(opengl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/glm)
target_include_directories(opengl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/headers)
target_include_directories(opengl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/stb)
target_compile_definitions(opengl PRIVATE GLM_ENABLE_EXPERIMENTAL)


target_link_libraries(opengl glfw OpenGL::GL)