#include "component.hpp"
#include "ecs/scene.hpp"

namespace fro
{
   Reference<Entity> Component::entity() const
   {
      return Reference{
         **std::ranges::find_if(scene_->entities_,
            [this](std::unique_ptr<Entity> const& entity)
            {
               return entity_id_ == static_cast<ID::InternalValue>(entity->id());
            })
      };
   }
}