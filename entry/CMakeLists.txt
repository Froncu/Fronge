set(FRO_ENTRY_NAME ${FRO_ENGINE_NAME}_entry)

add_library(${FRO_ENTRY_NAME} STATIC
   source/entry.cpp)

get_target_property(FRONGE_INCLUDE_DIRS ${FRO_ENGINE_NAME} INCLUDE_DIRECTORIES)
target_include_directories(${FRO_ENTRY_NAME}
   PRIVATE ${FRONGE_INCLUDE_DIRS})

target_link_libraries(${FRO_ENTRY_NAME}
   PRIVATE SDL3::SDL3)

set_target_properties(${FRO_ENTRY_NAME} PROPERTIES
   CXX_STANDARD 23
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${FRO_ENTRY_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
   PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)