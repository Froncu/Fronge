add_library(Fronge SHARED
   Source/Audio/Audio.cpp
   Source/ECS/Components/Rigidbody/Implementation/ColliderImpl.cpp
   Source/ECS/Components/Rigidbody/Implementation/RigidbodyImpl.cpp
   Source/ECS/Components/Rigidbody/Collider.cpp
   Source/ECS/Components/Rigidbody/Rigidbody.cpp
   Source/ECS/Components/Transform/Transform.cpp
   Source/ECS/Entity/EntityManager/EntityManager.cpp
   Source/ECS/Entity/Entity.cpp
   Source/ECS/Scene/SceneManager/SceneManager.cpp
   Source/ECS/Scene/Scene.cpp
   Source/ECS/Systems/PhysicsSystem/Implementation/PhysicsContactListener.cpp
   Source/ECS/Systems/PhysicsSystem/Implementation/PhysicsDebugRenderer.cpp
   Source/ECS/Systems/PhysicsSystem/Implementation/PhysicsSystemImpl.cpp
   Source/ECS/Systems/PhysicsSystem/PhysicsSystem.cpp
   Source/ECS/Systems/SpriteAnimatorSystem/SpriteAnimatorSystem.cpp
   Source/ECS/Systems/SpriteSystem/SpriteSystem.cpp
   Source/Events/AudioEvent.cpp
   Source/Events/InputEvent.cpp
   Source/Events/WindowEvent.cpp
   Source/Gamepad/Implementation/GamepadImpl.cpp
   Source/Gamepad/Gamepad.cpp
   Source/Idenitifer/ID.cpp
   Source/Idenitifer/IDGenerator.cpp
   Source/Input/Implementation/InputImpl.cpp
   Source/InputManager/InputManager.cpp
   Source/Logger/Logger.cpp
   Source/Maths/Structs/TransformMatrix/TransformMatrix3x3.cpp
   Source/Reference/BaseReference.cpp
   Source/Reference/Referencable.cpp
   Source/Renderer/Implementation/RendererImpl.cpp
   Source/Renderer/Implementation/SDL2_gfxPrimitives.cpp
   Source/Renderer/Renderer.cpp
   Source/Resources/Font/Implementation/FontImpl.cpp
   Source/Resources/Font/Font.cpp
   Source/Resources/Music/Implementation/MusicImpl.cpp
   Source/Resources/Music/Music.cpp
   Source/Resources/SoundEffect/Implementation/SoundEffectImpl.cpp
   Source/Resources/SoundEffect/SoundEffect.cpp
   Source/Resources/Surface/Implementation/SurfaceImpl.cpp
   Source/Resources/Surface/Surface.cpp
   Source/Resources/Texture/Implementation/TextureImpl.cpp
   Source/Resources/Texture/Texture.cpp
   Source/SVGParser/SVGParser.cpp
   Source/SystemEventManager/Implementation/SystemEventManagerImpl.cpp
   Source/SystemEventManager/SystemEventManager.cpp
   Source/Window/Implementation/WindowImpl.cpp
   Source/Window/Window.cpp
   Source/froch.cpp)

target_include_directories(Fronge PUBLIC
   Source)

target_precompile_headers(Fronge PRIVATE
   Source/froch.hpp)

target_compile_definitions(Fronge PRIVATE
   FRO_BUILD_DLL
   FRO_CONSOLE)

target_link_libraries(Fronge PRIVATE
   box2d
   SDL2::SDL2
   SDL2_ttf
   SDL2_image
   SDL2_mixer)

set_target_properties(Fronge PROPERTIES
   CXX_STANDARD 20
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(Fronge PRIVATE
   /W4
   /WX)