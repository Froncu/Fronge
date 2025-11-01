#include "fronge/entity.hpp"
#include "fronge/scene.hpp"

namespace fro
{
   Entity& Scene::create_entity() const
   {
      return *entities_.emplace_back(new Entity{ *this });
   }

   void Scene::destroy_entity(Entity const& entity) const
   {
      destroy_queue_.push_back(static_cast<ID::InternalValue>(entity.id()));
   }
}