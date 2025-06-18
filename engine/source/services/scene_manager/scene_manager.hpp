#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "core.hpp"
#include "ecs/scene.hpp"
#include "ecs/systems/sprite_renderer.hpp"
#include "froch.hpp"

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

         FRO_API Scene& add(std::string name);

         FRO_API void update(double delta_seconds);
         FRO_API void fixed_update(double fixed_delta_seconds);
         FRO_API void render();
         FRO_API void execute_queued();

      private:
         std::unordered_map<std::string, Scene> scenes_{};

         SpriteRenderer sprite_renderer_{};
   };
}

#endif