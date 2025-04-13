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
         static std::uint32_t constexpr INVALID_ID{ std::numeric_limits<std::uint32_t>::max() };

         FRO_API ID(ID&& other) noexcept;

         FRO_API ~ID();

         FRO_API ID& operator=(ID&& other) noexcept;
         FRO_API [[nodiscard]] explicit operator std::uint32_t() const;
         FRO_API [[nodiscard]] bool operator==(ID const& other) const;
         FRO_API [[nodiscard]] std::partial_ordering operator<=>(ID const& other) const;

         FRO_API [[nodiscard]] Reference<IDGenerator> generator() const;

      private:
         ID(Reference<IDGenerator> generator, std::uint32_t id);
         ID(ID const&) = delete;

         ID& operator=(ID const&) = delete;

         Reference<IDGenerator> generator_;
         std::uint32_t id_;
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
      if (static_cast<std::uint32_t>(id) == fro::ID::INVALID_ID)
         return std::format_to(context.out(), "{}", "INVALID");

      return std::format_to(context.out(), "{}", static_cast<std::uint32_t>(id));
   }
};

#endif