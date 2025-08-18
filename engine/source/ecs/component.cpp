#include "component.hpp"

namespace fro
{
   Reference<Entity> Component::entity() const
   {
      return entity_;
   }
}