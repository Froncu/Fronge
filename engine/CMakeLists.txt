add_library(${FRO_ENGINE_NAME} SHARED
   source/events/window_event.cpp
   source/services/system_event_dispatcher/system_event_dispatcher.cpp
   source/services/system_event_dispatcher/regular_system_event_dispatcher.cpp
   source/window/window.cpp
   source/logger/logger.cpp
   source/services/locator.cpp
   source/renderer/renderer.cpp
   source/resources/surface/surface.cpp
   source/renderer/texture.cpp
   source/services/locator.cpp
   source/reference/referenceable.cpp
   source/reference/base_reference.cpp
   source/services/locator.cpp
   source/window/renderer/renderer.cpp
   source/resources/surface/surface.cpp
   source/window/renderer/texture.cpp)

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