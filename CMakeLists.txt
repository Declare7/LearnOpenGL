cmake_minimum_required(VERSION 3.16)

project(OpenGLDemo LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/deps/bin)
add_subdirectory(deps)

find_package(glfw3 REQUIRED)

set(SOURCE_FILES
    main.cpp
    3rd/glad/src/glad.c
    3rd/stb/stb_image.cpp

    TriangleSection.h
    TriangleSection.cpp
    RectangleSection.h
    RectangleSection.cpp
    Uitility.h
    Uitility.cpp
    shader/TriangleSection/normal.vert
    shader/TriangleSection/normal.frag
    shader/TriangleSection/shader.vert
    shader/TriangleSection/shader.frag
    shader/RectangleSection/normal.vert
    shader/RectangleSection/normal.frag)

add_executable(OpenGLDemo ${SOURCE_FILES}
    SectionBase.h SectionBase.cpp
    ShaderSection.h ShaderSection.cpp
    shader/TriangleSection/uniform.frag
    shader/TriangleSection/uniform.vert
    shader/TriangleSection/colors.frag
    shader/TriangleSection/colors.vert)

target_link_libraries(OpenGLDemo PRIVATE glfw)

target_include_directories(OpenGLDemo PRIVATE 3rd/glad/include)
target_include_directories(OpenGLDemo PRIVATE 3rd/stb)

target_compile_definitions(OpenGLDemo PRIVATE SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
