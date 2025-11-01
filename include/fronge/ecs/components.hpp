#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "fronge/concepts/is_in_tuple.hpp"
#include "fronge/ecs/rigid_body.hpp"
#include "fronge/ecs/sprite.hpp"
#include "fronge/ecs/transform.hpp"
#include "fronge/pch.hpp"
#include "fronge/utility/sparse_set.hpp"

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