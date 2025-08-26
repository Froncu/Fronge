cmake_minimum_required(VERSION 3.15)

set(FRO_ENGINE_NAME fronge)
set(FRO_LIBRARY_NAME ${FRO_ENGINE_NAME}_library)
set(FRO_COMPILE_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

project(${FRO_ENGINE_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(external)

set(FRO_COMPILE_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(frogui)
add_subdirectory(library)
add_subdirectory(entry)