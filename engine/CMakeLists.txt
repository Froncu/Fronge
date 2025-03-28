add_library(${FRO_ENGINE_NAME} SHARED
   source/events/render_context_event.cpp
   source/logger/logger.cpp
   source/maths/transform_matrix.cpp
   source/reference/base_reference.cpp
   source/reference/referenceable.cpp
   source/render_context/render_context.cpp
   source/render_context/texture.cpp
   source/resources/surface/surface.cpp
   source/services/locator.cpp
   source/services/system_event_dispatcher/regular_system_event_dispatcher.cpp
   source/services/system_event_dispatcher/system_event_dispatcher.cpp)

target_include_directories(${FRO_ENGINE_NAME}
   PUBLIC source)

target_precompile_headers(${FRO_ENGINE_NAME}
   PRIVATE source/froch.hpp)

target_compile_definitions(${FRO_ENGINE_NAME}
   PRIVATE FRO_ENGINE
   PUBLIC FRO_CONSOLE)

target_link_libraries(${FRO_ENGINE_NAME}
   PRIVATE SDL3::SDL3
   PRIVATE SDL3_image::SDL3_image
   PRIVATE SDL3_ttf::SDL3_ttf
   PRIVATE SDL3_mixer::SDL3_mixer)

set_target_properties(${FRO_ENGINE_NAME} PROPERTIES
   CXX_STANDARD 20
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${FRO_ENGINE_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
   PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)