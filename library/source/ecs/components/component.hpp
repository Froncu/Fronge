#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "identifier/id.hpp"
#include "reference/reference.hpp"

namespace fro
{
   class Entity;
   class Scene;

   class Component
   {
      public:
         Component() = default;
         Component(Component const&) = delete;
         Component(Component&&) = default;

         virtual ~Component() = default;

         Component& operator=(Component const&) = delete;
         Component& operator=(Component&&) = default;

         Reference<Entity> entity() const;

      private:
         ID::InternalValue entity_id_{ ID::INVALID_ID };
         Reference<Scene const> scene_{};
   };
}

#endif