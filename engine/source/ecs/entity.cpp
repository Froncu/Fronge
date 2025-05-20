#include "entity.hpp"

namespace fro
{
   IDGenerator Entity::id_generator_{};

   Entity::Entity(Scene& scene)
      : scene_{ scene }
   {
   }

   Entity::~Entity()
   {
      std::apply([entity_id = ID::InternalValue{ id() }](auto&... component_sparse_sets)
      {
         (component_sparse_sets.enqueue_remove(entity_id), ...);
      }, scene_->component_sparse_sets_);
   }
}