#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "components/rigid_body.hpp"
#include "components/sprite.hpp"
#include "components/transform.hpp"
#include "froch.hpp"
#include "utility/is_in_tuple.hpp"
#include "utility/sparse_set.hpp"

namespace fro
{
   using Components = std::tuple<
      RigidBody,
      Transform,
      Sprite>;

   template <typename Type>
   concept Componentable =
      SparseSetStorable<Type> and
      IsInTuple<Components, Type>;
}

#endif