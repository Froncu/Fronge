#ifndef BASE_REFERENCE_HPP
#define BASE_REFERENCE_HPP

#include "fronge/api.hpp"
#include "fronge/pch.hpp"

namespace fro
{
   class Referenceable;

   class BaseReference
   {
      friend Referenceable;

      public:
         FRO_API virtual ~BaseReference();

         FRO_API [[nodiscard]] bool operator==(Referenceable const& referenceable) const;
         FRO_API [[nodiscard]] bool operator==(BaseReference const& other) const;
         FRO_API [[nodiscard]] std::strong_ordering operator<=>(BaseReference const& other) const;
         FRO_API [[nodiscard]] std::strong_ordering operator<=>(Referenceable const& referenceable) const;

         FRO_API void reset();
         FRO_API bool valid() const;

      protected:
         BaseReference() = default;
         FRO_API explicit BaseReference(Referenceable const* referenceable);
         FRO_API explicit BaseReference(Referenceable const& referenceable);
         FRO_API BaseReference(BaseReference const& other);
         FRO_API BaseReference(BaseReference&& other) noexcept;

         FRO_API BaseReference& operator=(BaseReference&& other) noexcept;
         FRO_API BaseReference& operator=(BaseReference const& other);

         Referenceable* referenceable_{};
   };
}

#endif