#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include "core.hpp"
#include "froch.hpp"
#include "reference/referenceable.hpp"

namespace fro
{
   class ID;

   class IDGenerator final : public Referenceable
   {
      friend ID;

      public:
         FRO_API IDGenerator();
         FRO_API IDGenerator(IDGenerator&& other) noexcept;

         virtual ~IDGenerator() override = default;

         FRO_API IDGenerator& operator=(IDGenerator&& other) noexcept;

         FRO_API [[nodiscard]] ID generate();
         FRO_API [[nodiscard]] std::uint32_t highest_taken_id() const;

      private:
         IDGenerator(IDGenerator const&) = delete;

         IDGenerator& operator=(IDGenerator const&) = delete;

         // NOTE: not initializing here as id.hpp cannot be included here
         std::uint32_t highest_taken_id_;
         std::set<std::uint32_t, std::greater<std::uint32_t>> free_ids_{};
   };
}

#endif