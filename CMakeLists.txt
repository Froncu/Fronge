cmake_minimum_required(VERSION 3.15)
project(Fronge)

if (NOT MSVC)
   message(FATAL_ERROR "This project currently can be build only using Microsoft Visual C++ (MSVC).")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Libraries)

add_subdirectory(Fronge)
add_subdirectory(FrongeEntry)