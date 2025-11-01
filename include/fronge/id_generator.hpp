#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include "fronge/api.hpp"
#include "fronge/pch.hpp"
#include "fronge/referenceable.hpp"

namespace fro
{
   class ID;

   class IDGenerator final : public Referenceable
   {
      friend ID;

      using InternalValue = std::uint32_t;
      static InternalValue constexpr INVALID_ID{ std::numeric_limits<InternalValue>::max() };
      static InternalValue constexpr MAX_ID{ INVALID_ID - 1 };

      public:
         IDGenerator() = default;
         IDGenerator(IDGenerator const&) = delete;
         FRO_API IDGenerator(IDGenerator&& other) noexcept;

         virtual ~IDGenerator() override = default;

         IDGenerator& operator=(IDGenerator const&) = delete;
         FRO_API IDGenerator& operator=(IDGenerator&& other) noexcept;

         FRO_API [[nodiscard]] ID generate();

      private:
         [[nodiscard]] InternalValue internal_generate();

         InternalValue highest_taken_id_{ INVALID_ID };
         std::set<InternalValue, std::greater<InternalValue>> free_ids_{};
   };
}

#endif