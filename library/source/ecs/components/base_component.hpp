#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

#include "identifier/id.hpp"
#include "reference/reference.hpp"
#include "reference/referenceable.hpp"

namespace fro
{
   class Entity;
   class Scene;

   class BaseComponent : public Referenceable
   {
      public:
         BaseComponent() = default;
         BaseComponent(BaseComponent const&) = delete;
         BaseComponent(BaseComponent&&) = default;

         virtual ~BaseComponent() override = default;

         BaseComponent& operator=(BaseComponent const&) = delete;
         BaseComponent& operator=(BaseComponent&&) = default;

         Entity& entity() const;

      private:
         ID::InternalValue entity_id_{ ID::INVALID_ID };
         Reference<Scene const> scene_{};
   };
}

#endif