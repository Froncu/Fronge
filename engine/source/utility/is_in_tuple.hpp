#ifndef IS_IN_TUPLE_HPP
#define IS_IN_TUPLE_HPP

#include "froch.hpp"

namespace fro
{
   template<typename Type, typename Tuple>
   struct IsInTupleHelper;

   template<typename Type, typename... TupleTypes>
   struct IsInTupleHelper<Type, std::tuple<TupleTypes...>> : std::disjunction<std::is_same<Type, TupleTypes>...> {};

   template<typename Tuple, typename... Types>
   concept IsInTuple = (IsInTupleHelper<Types, Tuple>::value && ...);
}

#endif