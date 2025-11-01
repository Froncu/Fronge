#ifndef UNIQUE_PARAMETER_PACK_HPP
#define UNIQUE_PARAMETER_PACK_HPP

#include "fronge/pch.hpp"

namespace fro
{
   template <typename...>
   static auto constexpr IS_UNIQUE{ std::true_type{} };

   template <typename Head, typename... Tail>
   static auto constexpr IS_UNIQUE<Head, Tail...>
   {
      (not std::is_same_v<Head, Tail> and ...) and IS_UNIQUE<Tail...>
   };
}

#endif