add_library(${FRO_ENGINE_NAME} SHARED
   source/application/application.cpp
   source/ecs/components/transform.cpp
   source/ecs/entity.cpp
   source/ecs/scene.cpp
   source/ecs/systems/sprite_renderer.cpp
   source/identifier/id.cpp
   source/identifier/id_generator.cpp
   source/input/gamepad.cpp
   source/input/input_conversions.cpp
   source/input/user_input.cpp
   source/maths/transform_matrix.cpp
   source/reference/base_reference.cpp
   source/reference/referenceable.cpp
   source/resources/surface/surface.cpp
   source/services/input_manager/input_manager.cpp
   source/services/locator.cpp
   source/services/logger/coloured_logger.cpp
   source/services/logger/disabled_logger.cpp
   source/services/logger/logger.cpp
   source/services/render_context/render_context.cpp
   source/services/render_context/texture.cpp
   source/services/scene_manager/scene_manager.cpp
   source/services/system_event_dispatcher/system_event_dispatcher.cpp)

target_include_directories(${FRO_ENGINE_NAME}
   PUBLIC source)

target_precompile_headers(${FRO_ENGINE_NAME}
   PRIVATE source/froch.hpp)

target_compile_definitions(${FRO_ENGINE_NAME}
   PRIVATE FRO_ENGINE
   PUBLIC FRO_LOGGER)

target_link_libraries(${FRO_ENGINE_NAME}
   PRIVATE SDL3::SDL3
   PRIVATE SDL3_image::SDL3_image
   PRIVATE SDL3_ttf::SDL3_ttf
   PRIVATE SDL3_mixer::SDL3_mixer
   PRIVATE box2d::box2d)

set_target_properties(${FRO_ENGINE_NAME} PROPERTIES
   CXX_STANDARD 23
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${FRO_ENGINE_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
   PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)