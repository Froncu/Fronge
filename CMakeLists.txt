cmake_minimum_required(VERSION 3.30)
project(Fronge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")

add_subdirectory(Fronge)
add_subdirectory(FrongeEditor)