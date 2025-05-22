#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "components/sprite.hpp"
#include "components/transform.hpp"
#include "froch.hpp"
#include "utility/is_in_tuple.hpp"
#include "utility/sparse_set.hpp"

namespace fro
{
   using Components = std::tuple<
      Transform,
      Sprite>;

   template <typename Type>
   concept Componentable =
      SparseSetStorable<Type> and
      IsInTuple<Components, Type>;
}

#endif