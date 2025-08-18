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

   #ifdef __MINGW32__
   auto constexpr MINGW{ true };
   #else
   auto constexpr MINGW{ false };
   #endif
}

#endif