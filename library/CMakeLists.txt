add_library(${FRO_LIBRARY_NAME} SHARED
   source/application/application.cpp
   source/ecs/components/transform.cpp
   source/ecs/entity.cpp
   source/ecs/scene.cpp
   source/ecs/systems/physics_system.cpp
   source/ecs/systems/sprite_renderer.cpp
   source/identifier/id.cpp
   source/identifier/id_generator.cpp
   source/input/gamepad.cpp
   source/input/input_conversions.cpp
   source/input/user_input.cpp
   source/maths/polygon/polygon.cpp
   source/maths/transform_matrix/transform_matrix.cpp
   source/reference/base_reference.cpp
   source/reference/referenceable.cpp
   source/resources/surface/surface.cpp
   source/services/editor_ui/editor_ui.cpp
   source/services/input_manager/input_manager.cpp
   source/services/locator.cpp
   source/services/logger/logger.cpp
   source/services/renderer/renderer.cpp
   source/services/renderer/texture.cpp
   source/services/scene_manager/scene_manager.cpp
   source/services/system_event_dispatcher/system_event_dispatcher.cpp
   source/services/window/window.cpp)

target_include_directories(${FRO_LIBRARY_NAME}
   PUBLIC source)

target_precompile_headers(${FRO_LIBRARY_NAME}
   PRIVATE source/froch.hpp)

target_compile_definitions(${FRO_LIBRARY_NAME}
   PRIVATE FRO_ENGINE
   PRIVATE FRO_COMPILE_SOURCE_PATH="${FRO_COMPILE_SOURCE_PATH}")

target_link_libraries(${FRO_LIBRARY_NAME}
   PRIVATE SDL3::SDL3-static
   PRIVATE SDL3_image::SDL3_image-static
   PRIVATE imgui::imgui)

set_target_properties(${FRO_LIBRARY_NAME} PROPERTIES
   CXX_STANDARD 23
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${FRO_LIBRARY_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
   PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)