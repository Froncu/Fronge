#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "identifier/id.hpp"

namespace fro
{
   class Entity final
   {
      public:
         Entity() = default;
         Entity(Entity const&) = default;
         Entity(Entity&&) = default;

         ~Entity() = default;

         Entity& operator=(Entity const&) = default;
         Entity& operator=(Entity&&) = default;

      private:
         static IDGenerator id_generator_;

         ID id_{ id_generator_.generate() };
   };
}

#endif