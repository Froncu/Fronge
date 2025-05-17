#include "entity.hpp"
#include "scene.hpp"

namespace fro
{
   void Scene::BaseComponentSparseSet::enqueue_remove(ID::InternalValue const entity_id)
   {
      remove_queue_.insert(entity_id);
   }

   Entity& Scene::create_entity()
   {
      return *entities_.emplace_back(new Entity{ *this });
   }

   void Scene::destroy_entity(Entity const& entity)
   {
      destroy_queue_.push_back(&entity);
   }

   void Scene::execute_queued()
   {
      for (Entity const* entity : destroy_queue_)
         std::erase_if(entities_,
            [entity](std::unique_ptr<Entity> const& stored_entity)
            {
               return stored_entity.get() == entity;
            });
      destroy_queue_.clear();

      for (std::unique_ptr<BaseComponentSparseSet> const& component_sparse_set :
           std::views::values(component_sparse_sets_))
         component_sparse_set->remove_add_queued();
   }
}