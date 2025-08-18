#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "core.hpp"
#include "reference/reference.hpp"
#include "reference/referenceable.hpp"

namespace fro
{
   class Entity;
   class Scene;

   class Component : public Referenceable
   {
      friend Scene;

      public:
         Component() = default;
         Component(Component const&) = default;
         Component(Component&&) = default;

         virtual ~Component() override = default;

         Component& operator=(Component const&) = default;
         Component& operator=(Component&&) = default;

         FRO_API Reference<Entity> entity() const;

      private:
         Reference<Entity> entity_{};
   };
}

#endif