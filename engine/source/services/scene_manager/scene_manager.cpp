#include "scene_manager.hpp"

namespace fro
{
   Scene& SceneManager::add(std::string name)
   {
      auto scene{ scenes_.find(name) };
      if (scene == scenes_.end())
         scene = scenes_.emplace(std::move(name), Scene{}).first;

      return scene->second;
   }

   void SceneManager::update(double /*delta_seconds*/)
   {
   }

   void SceneManager::fixed_update(double /*fixed_delta_seconds*/)
   {
   }

   void SceneManager::render()
   {
      for (Scene& scene : std::views::values(scenes_))
         sprite_renderer_.render(scene);
   }

   void SceneManager::execute_queued()
   {
      for (Scene& scene : std::views::values(scenes_))
      {
         for (ID::InternalValue const entity_id : scene.destroy_queue_)
            std::erase_if(scene.entities_,
               [entity_id](std::unique_ptr<Entity> const& entity)
               {
                  return static_cast<ID::InternalValue>(entity->id()) == entity_id;
               });
         scene.destroy_queue_.clear();

         bool did_any_change{};
         std::apply([&did_any_change](auto&... component_sparse_sets)
         {
            ((did_any_change = component_sparse_sets.remove_add_queued() or did_any_change), ...);
         }, scene.component_sparse_sets_);

         if (did_any_change)
            for (std::unique_ptr<Scene::BaseGroup> const& group : std::views::values(scene.groups_))
               group->update();
      }
   }
}