cmake_minimum_required(VERSION 3.15)
set(FRO_ENGINE_NAME fronge)

project(${FRO_ENGINE_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(libraries)

add_subdirectory(engine)
add_subdirectory(entry)