#ifndef ID_HPP
#define ID_HPP

#include "core.hpp"
#include "froch.hpp"
#include "id_generator.hpp"
#include "reference/reference.hpp"

namespace fro
{
   class ID final
   {
      friend IDGenerator;

      public:
         using InternalValue = IDGenerator::InternalValue;
         static InternalValue constexpr INVALID_ID{ IDGenerator::INVALID_ID };
         static InternalValue constexpr MAX_ID{ IDGenerator::MAX_ID };

         FRO_API ID(ID const& other);
         FRO_API ID(ID&& other) noexcept;

         FRO_API ~ID();

         FRO_API ID& operator=(ID const& other);
         FRO_API ID& operator=(ID&& other) noexcept(false);
         FRO_API [[nodiscard]] explicit operator InternalValue() const;
         FRO_API [[nodiscard]] bool operator==(ID const& other) const;
         FRO_API [[nodiscard]] std::partial_ordering operator<=>(ID const& other) const;

         FRO_API [[nodiscard]] Reference<IDGenerator> generator() const;

      private:
         explicit ID(IDGenerator& generator);

         void free_value() const;

         Reference<IDGenerator> generator_;
         InternalValue value_;
   };
}

template <>
struct std::formatter<fro::ID>
{
   static constexpr auto parse(std::format_parse_context const& context)
   {
      return context.begin();
   }

   static auto format(fro::ID const& id, std::format_context& context)
   {
      if (static_cast<fro::ID::InternalValue>(id) == fro::ID::INVALID_ID)
         return std::format_to(context.out(), "{}", "INVALID");

      return std::format_to(context.out(), "{}", static_cast<fro::ID::InternalValue>(id));
   }
};

#endif