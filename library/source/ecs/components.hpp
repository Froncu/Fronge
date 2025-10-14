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
      std::derived_from<BaseComponent, Type> and
      SparseSetStorable<Type> and
      IsInTuple<Components, Type>;
}

#endif