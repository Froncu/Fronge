add_library(${FRO_ENGINE_NAME} STATIC
   source/entry.cpp)

target_link_libraries(${FRO_ENGINE_NAME}
   PUBLIC ${FRO_LIBRARY_NAME}
   PRIVATE SDL3::SDL3-static)

set_target_properties(${FRO_ENGINE_NAME} PROPERTIES
   CXX_STANDARD 23
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${FRO_ENGINE_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
   PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)