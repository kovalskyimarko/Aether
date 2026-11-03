cmake_minimum_required(VERSION 3.14)
project(Aether CXX C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp" "src/*.c")
add_executable(${PROJECT_NAME}
    preview/main.cpp
    ${SOURCES}
    libs/glad/src/glad.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image"
)

find_package(glfw3 3.3 QUIET)

if(glfw3_FOUND)
    message(STATUS "Using system GLFW ${glfw3_VERSION}")
else()
    message(STATUS "System GLFW not found - fetching GLFW 3.4 from GitHub")
    include(FetchContent)

    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(glfw)
endif()

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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/preview"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/preview"
    COMMENT "Copying preview assets to build directory..."
)