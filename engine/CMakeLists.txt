add_library(${FRO_ENGINE_NAME} SHARED
   source/audio/audio.cpp
   source/ecs/components/transform/transform.cpp
   source/ecs/entity/entity_manager/entity_manager.cpp
   source/ecs/entity/entity.cpp
   source/ecs/scene/scene_manager/scene_manager.cpp
   source/ecs/scene/scene.cpp
   source/ecs/systems/sprite_animator_system/sprite_animator_system.cpp
   source/ecs/systems/sprite_system/sprite_system.cpp
   source/events/audio_event.cpp
   source/events/input_event.cpp
   source/events/window_event.cpp
   source/gamepad/implementation/gamepad_impl.cpp
   source/gamepad/gamepad.cpp
   source/identifier/id.cpp
   source/identifier/id_generator.cpp
   source/input/implementation/input_impl.cpp
   source/input_manager/input_manager.cpp
   source/logger/logger.cpp
   source/maths/structs/transform_matrix_3x3/transform_matrix_3x3.cpp
   source/reference/base_reference.cpp
   source/reference/referencable.cpp
   source/renderer/implementation/renderer_impl.cpp
   source/renderer/renderer.cpp
   source/resources/font/implementation/font_impl.cpp
   source/resources/font/font.cpp
   source/resources/music/implementation/music_impl.cpp
   source/resources/music/music.cpp
   source/resources/sound_effect/implementation/sound_effect_impl.cpp
   source/resources/sound_effect/sound_effect.cpp
   source/resources/surface/implementation/surface_impl.cpp
   source/resources/surface/surface.cpp
   source/resources/texture/implementation/texture_impl.cpp
   source/resources/texture/texture.cpp
   source/system_event_manager/implementation/system_event_manager_impl.cpp
   source/system_event_manager/system_event_manager.cpp
   source/window/implementation/window_impl.cpp
   source/window/window.cpp)

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
   PRIVATE /W4
   PRIVATE /WX)