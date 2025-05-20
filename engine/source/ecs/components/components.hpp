#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "froch.hpp"
#include "utility/is_in_tuple.hpp"
#include "utility/sparse_set.hpp"

namespace fro
{
   using Components = std::tuple<
      int,
      char,
      float>;

   template <typename Type>
   concept Componentable =
      SparseSetStorable<Type> and
      IsInTuple<Components, Type>;
}

#endif