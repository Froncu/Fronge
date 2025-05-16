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
      for (std::unique_ptr<Scene::BaseComponentSparseSet> const& component_sparse_set :
           std::views::values(scene_->component_sparse_sets_))
         component_sparse_set->remove_component(static_cast<std::uint32_t>(id_));
   }
}