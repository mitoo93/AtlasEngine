cmake_minimum_required(VERSION 3.20)

project(AtlasEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(External/glfw)

add_subdirectory(External/glad)

add_subdirectory(External/imgui)

add_subdirectory(Engine)

add_subdirectory(Sandbox)