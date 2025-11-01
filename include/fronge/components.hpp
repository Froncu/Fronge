#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "fronge/is_in_tuple.hpp"
#include "fronge/pch.hpp"
#include "fronge/rigid_body.hpp"
#include "fronge/sparse_set.hpp"
#include "fronge/sprite.hpp"
#include "fronge/transform.hpp"

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