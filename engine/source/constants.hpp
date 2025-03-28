#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace fro
{
   #ifdef NDEBUG
   auto constexpr DEBUG{ false };
   #else
   auto constexpr DEBUG{ true };
   #endif

   #ifdef FRO_ENGINE
   auto constexpr ENGINE{ true };
   #else
   auto constexpr ENGINE{ false };
   #endif

   #ifdef FRO_LOGGER
   auto constexpr LOGGER{ true };
   #else
   auto constexpr LOGGER{ false };
   #endif
};

#endif