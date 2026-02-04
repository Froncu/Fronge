#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "fronge/pch.hpp"

namespace fro
{
   #ifdef NDEBUG
   auto constexpr DEBUG{ false };
   #else
   auto constexpr DEBUG{ true };
   #endif

   #ifdef FRO_ENGINE_LEVEL
   auto constexpr ENGINE_LEVEL{ true };
   std::string_view constexpr COMPILE_SOURCE_PATH{ FRO_COMPILE_SOURCE_PATH };
   #else
   auto constexpr ENGINE_LEVEL{ false };
   #endif

   #ifdef __MINGW32__
   auto constexpr MINGW{ true };
   #else
   auto constexpr MINGW{ false };
   #endif
}

#endif