cmake_minimum_required(VERSION 3.16)

project(deps)

include(ExternalProject)

set(DESTDIR ${CMAKE_PREFIX_PATH})

include(GLFW/GLFW.cmake)
include(GLM/GLM.cmake)
