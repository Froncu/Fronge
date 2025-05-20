#include "entity.hpp"
#include "scene.hpp"

namespace fro
{
   Entity& Scene::create_entity()
   {
      return *entities_.emplace_back(new Entity{ *this });
   }

   void Scene::destroy_entity(Entity const& entity)
   {
      destroy_queue_.push_back(static_cast<ID::InternalValue>(entity.id()));
   }

   void Scene::execute_queued()
   {
      for (ID::InternalValue entity_id : destroy_queue_)
         std::erase_if(entities_,
            [entity_id](std::unique_ptr<Entity> const& entity)
            {
               return static_cast<ID::InternalValue>(entity->id()) == entity_id;
            });
      destroy_queue_.clear();

      std::apply([](auto&... component_sparse_sets)
      {
         (component_sparse_sets.remove_add_queued(), ...);
      }, component_sparse_sets_);
   }
}