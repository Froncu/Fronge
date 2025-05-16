#include "entity.hpp"
#include "scene.hpp"

namespace fro
{
   Entity& Scene::create_entity()
   {
      return *entities_.emplace_back(new Entity{ *this });
   }
}