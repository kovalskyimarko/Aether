cmake_minimum_required(VERSION 3.10)
project(Aether CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp" "src/*.c")
add_executable(${PROJECT_NAME}
    ${SOURCES}
    libs/glad/src/glad.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image"
)

find_package(glfw3 REQUIRED)

if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        ${OPENGL_LIBRARY}
    )

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        add_compile_options(-arch arm64)
    endif()
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        OpenGL::GL
    )
endif()