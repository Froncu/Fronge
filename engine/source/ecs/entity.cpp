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
      remove_components();
   }
}