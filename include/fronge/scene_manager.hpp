#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "fronge/api.hpp"
#include "fronge/pch.hpp"
#include "fronge/physics_system.hpp"
#include "fronge/scene.hpp"
#include "fronge/sprite_renderer.hpp"

namespace fro
{
   class SceneManager final
   {
      public:
         SceneManager() = default;
         SceneManager(SceneManager const&) = delete;
         SceneManager(SceneManager&&) noexcept = default;

         ~SceneManager() = default;

         SceneManager& operator=(SceneManager const&) = delete;
         SceneManager& operator=(SceneManager&&) noexcept = default;

         FRO_API Scene const& add(std::string name);

         FRO_API void update(double delta_seconds);
         FRO_API void fixed_update(double fixed_delta_seconds);
         FRO_API void render();
         FRO_API void execute_queued();

      private:
         std::unordered_map<std::string, Scene> scenes_{};

         SpriteRenderer sprite_renderer_{};
         PhysicsSystem physics_system_{};
   };
}

#endif