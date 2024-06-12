cmake_minimum_required(VERSION 3.29)
project(Fronge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")

set(GLOBAL_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/${PRESET_NAME})

add_subdirectory(Fronge)
add_subdirectory(FrongeEditor)