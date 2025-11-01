cmake_minimum_required(VERSION 3.15)
project(fronge)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(project_defaults)
include(umbrella_header)

find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-static)
find_package(SDL3_image CONFIG REQUIRED COMPONENTS SDL3_image-static)
find_package(imgui CONFIG REQUIRED COMPONENTS imgui-static)


set(FRO_LIBRARY_NAME ${PROJECT_NAME}_library)

file(GLOB_RECURSE FRO_SOURCE_FILES CONFIGURE_DEPENDS source/*.cpp)
list(FILTER FRO_SOURCE_FILES EXCLUDE REGEX entry\\.cpp)
add_library(${FRO_LIBRARY_NAME} SHARED
   ${FRO_SOURCE_FILES})

target_include_directories(${FRO_LIBRARY_NAME}
   PUBLIC include
   PRIVATE source)

target_precompile_headers(${FRO_LIBRARY_NAME}
   PRIVATE include/${PROJECT_NAME}/pch.hpp)

target_compile_definitions(${FRO_LIBRARY_NAME}
   PRIVATE FRO_ENGINE
   PRIVATE FRO_COMPILE_SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${FRO_LIBRARY_NAME}
   PRIVATE SDL3::SDL3-static
   PRIVATE SDL3_image::SDL3_image-static
   PRIVATE imgui::imgui)

set_project_defaults(${FRO_LIBRARY_NAME})
generate_umbrella_header(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME})


add_library(${PROJECT_NAME} STATIC
   source/entry.cpp)

target_link_libraries(${PROJECT_NAME}
   PUBLIC ${FRO_LIBRARY_NAME}
   PRIVATE SDL3::SDL3-static)

set_project_defaults(${PROJECT_NAME})