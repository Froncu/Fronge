cmake_minimum_required(VERSION 3.15)
project(fronge)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(project_defaults)
include(umbrella_header)

option(FRO_BUILD_SHARED "Build ${PROJECT_NAME} as shared library" ${BUILD_SHARED_LIBS})

find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-static)
find_package(SDL3_image CONFIG REQUIRED COMPONENTS SDL3_image-static)

# Engine library - contains all core engine functionality
# Can be built as SHARED or STATIC depending on FRO_BUILD_SHARED
# Direct linking is possible but not recommended; use the entry point library instead unless you know what you are doing

file(GLOB_RECURSE FRO_SOURCE_FILES CONFIGURE_DEPENDS source/*.cpp)
list(FILTER FRO_SOURCE_FILES EXCLUDE REGEX entry\\.cpp)
if(FRO_BUILD_SHARED)
   add_library(${PROJECT_NAME} SHARED ${FRO_SOURCE_FILES})
else()
   add_library(${PROJECT_NAME} STATIC ${FRO_SOURCE_FILES})
endif()

target_include_directories(${PROJECT_NAME}
   PUBLIC include
   PRIVATE source)

target_precompile_headers(${PROJECT_NAME}
   PRIVATE include/${PROJECT_NAME}/pch.hpp)

target_compile_definitions(${PROJECT_NAME}
   PUBLIC $<$<BOOL:${FRO_BUILD_SHARED}>:FRO_SHARED>
   PRIVATE FRO_ENGINE_LEVEL
   PRIVATE FRO_COMPILE_SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(${PROJECT_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-fvisibility=hidden -fvisibility-inlines-hidden>)

target_link_libraries(${PROJECT_NAME}
   PRIVATE SDL3::SDL3
   PRIVATE SDL3_image::SDL3_image)

target_link_options(${PROJECT_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/EXPORT:SDL_RunApp /EXPORT:SDL_EnterAppMainCallbacks>)

set_project_defaults(${PROJECT_NAME})
generate_umbrella_header(${PROJECT_NAME}
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME})

# Entry point library - provides the application entry point
# This is the recommended target for executables to link against
# Automatically includes the engine library as a transitive dependency

set(FRO_ENTRY_NAME ${PROJECT_NAME}_entry)

add_library(${FRO_ENTRY_NAME} STATIC
   source/core/entry.cpp)

target_link_libraries(${FRO_ENTRY_NAME}
   PUBLIC ${PROJECT_NAME}
   PRIVATE SDL3::Headers)

set_project_defaults(${FRO_ENTRY_NAME})