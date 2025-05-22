#ifndef MAPPED_TUPLE_HPP
#define MAPPED_TUPLE_HPP

#include "froch.hpp"

namespace fro
{
   template <template <typename> class Wrapper, typename Tuple>
   struct MappedTuple;

   template <template <typename> class Wrapper, typename... TupleTypes>
   struct MappedTuple<Wrapper, std::tuple<TupleTypes...>>
   {
      using Type = std::tuple<Wrapper<TupleTypes>...>;
   };
}

#endif