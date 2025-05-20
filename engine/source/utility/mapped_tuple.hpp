#ifndef MAPPED_TUPLE_HPP
#define MAPPED_TUPLE_HPP

#include "froch.hpp"

namespace fro
{
   template <template <typename> class Wrapper, typename Tuple>
   struct MappedTuple;

   template <template <typename> class Wrapper, typename... Ts>
   struct MappedTuple<Wrapper, std::tuple<Ts...>>
   {
      using Type = std::tuple<Wrapper<Ts>...>;
   };
}

#endif