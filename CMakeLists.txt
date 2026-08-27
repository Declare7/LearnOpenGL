cmake_minimum_required(VERSION 3.16)

project(OpenGLDemo LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/deps/bin)

execute_process(
    COMMAND_ECHO STDOUT
    COMMAND ${CMAKE_COMMAND}
        -S "${CMAKE_CURRENT_SOURCE_DIR}/deps"
        -B "${CMAKE_CURRENT_BINARY_DIR}/deps"
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_C_COMPILER_ARG1=${CMAKE_C_COMPILER_ARG1}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_COMPILER_ARG1=${CMAKE_CXX_COMPILER_ARG1}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
        -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
        -DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}
        -DCMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}
        -DCMAKE_C_FLAGS_RELWITHDEBINFO=${CMAKE_C_FLAGS_RELWITHDEBINFO}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
        -DCMAKE_CXX_FLAGS_RELWITHDEBINFO=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}

    RESULT_VARIABLE configure_result
    OUTPUT_VARIABLE configure_output
    ERROR_VARIABLE configure_error
)
if(NOT configure_result EQUAL 0)
    message(FATAL_ERROR "Failed to configure dependencies:\n${configure_error}")
endif()

execute_process(
    COMMAND_ECHO STDOUT
    COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/deps"
)

find_package(glfw3 REQUIRED)

set(SOURCE_FILES
    main.cpp
    3rd/glad/src/glad.c
    3rd/stb/stb_image.cpp

    SectionBase.h
    SectionBase.cpp
    TriangleSection.h
    TriangleSection.cpp
    RectangleSection.h
    RectangleSection.cpp
    TransformSection.h
    TransformSection.cpp
    CoordinateSection.h
    CoordinateSection.cpp
    CameraSection.h
    CameraSection.cpp

    shader/TriangleSection/normal.vert
    shader/TriangleSection/normal.frag
    shader/TriangleSection/shader.vert
    shader/TriangleSection/shader.frag
    shader/TriangleSection/uniform.frag
    shader/TriangleSection/uniform.vert
    shader/TriangleSection/colors.frag
    shader/TriangleSection/colors.vert

    shader/RectangleSection/normal.vert
    shader/RectangleSection/normal.frag
    shader/RectangleSection/EBO.vert
    shader/RectangleSection/EBO.frag
    shader/RectangleSection/texture.frag
    shader/RectangleSection/texture.vert
    shader/RectangleSection/textureUnit.frag
    shader/RectangleSection/textureUnit.vert
    shader/TransformSection/normal.frag
    shader/TransformSection/normal.vert
    shader/CoordinateSection/normal.frag
    shader/CoordinateSection/normal.vert
    shader/CoordinateSection/3D.frag
    shader/CoordinateSection/3D.vert
    shader/CameraSection/normal.frag
    shader/CameraSection/normal.vert)

add_executable(OpenGLDemo ${SOURCE_FILES}
    ShaderProgram.h ShaderProgram.cpp)

target_link_libraries(OpenGLDemo PRIVATE glfw)

target_include_directories(OpenGLDemo PRIVATE 3rd/glad/include)
target_include_directories(OpenGLDemo PRIVATE 3rd/stb)

target_compile_definitions(OpenGLDemo PRIVATE SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
