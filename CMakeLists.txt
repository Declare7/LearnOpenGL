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
    3rd/stb/stb_image.cpp)

add_executable(OpenGLDemo ${SOURCE_FILES})

target_link_libraries(OpenGLDemo PRIVATE glfw)

target_include_directories(OpenGLDemo PRIVATE 3rd/glad/include)
target_include_directories(OpenGLDemo PRIVATE 3rd/stb)
