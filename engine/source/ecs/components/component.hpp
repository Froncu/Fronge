#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "core.hpp"
#include "events/systems/event_listener.hpp"
#include "reference/reference.hpp"

namespace fro
{
   class Component : public Referencable
   {
      public:
         virtual ~Component() override = default;

      protected:
         Component() = default;
         Component(Component const&) = default;
         Component(Component&&) noexcept = default;

         Component& operator=(Component const&) = default;
         Component& operator=(Component&&) noexcept = default;
   };
}

#endif