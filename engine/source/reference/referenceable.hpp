#ifndef REFERENCABLE_HPP
#define REFERENCABLE_HPP

#include "core.hpp"
#include "froch.hpp"

namespace fro
{
   class Referenceable
   {
      friend class BaseReference;

      public:
         FRO_API virtual ~Referenceable();

      protected:
         Referenceable() = default;
         FRO_API Referenceable(Referenceable const&);
         FRO_API Referenceable(Referenceable&& other) noexcept;

         FRO_API Referenceable& operator=(Referenceable const&);
         FRO_API Referenceable& operator=(Referenceable&& other) noexcept;

      private:
         std::unordered_set<BaseReference*> references_{};
   };
}

#endif