cmake_minimum_required(VERSION 3.15)

set(FRO_ENGINE_NAME fronge)
set(FRO_LIBRARY_NAME ${FRO_ENGINE_NAME}_library)
set(FRO_COMPILE_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

project(${FRO_ENGINE_NAME})

find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-static)
find_package(SDL3_image CONFIG REQUIRED COMPONENTS SDL3_image-static)
find_package(imgui CONFIG REQUIRED COMPONENTS imgui-static)

#include(FetchContent)
#set(FETCHCONTENT_QUIET OFF)
#FetchContent_Declare(frogui
#   GIT_REPOSITORY https://github.com/Froncu/frogui
#   GIT_TAG fronge
#   GIT_PROGRESS ON
#   GIT_SHALLOW ON)
#FetchContent_MakeAvailable(frogui)

add_subdirectory(library)
add_subdirectory(entry)