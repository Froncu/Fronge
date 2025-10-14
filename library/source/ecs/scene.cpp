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
}