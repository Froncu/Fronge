set(FRO_ENTRY_NAME ${FRO_ENGINE_NAME}_entry)

add_library(${FRO_ENTRY_NAME} STATIC
   source/entry.cpp)

get_target_property(FRONGE_INCLUDE_DIRS ${FRO_ENGINE_NAME} INCLUDE_DIRECTORIES)
target_include_directories(${FRO_ENTRY_NAME}
   PRIVATE ${FRONGE_INCLUDE_DIRS})

target_link_libraries(${FRO_ENTRY_NAME}
   PRIVATE SDL2::SDL2
   PRIVATE SDL2::SDL2main)

set_target_properties(${FRO_ENTRY_NAME} PROPERTIES
   CXX_STANDARD 20
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${FRO_ENTRY_NAME}
   PRIVATE /W4
   PRIVATE /WX)