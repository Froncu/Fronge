#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "maths/shapes.hpp"
#include "maths/transform_matrix.hpp"
#include "maths/vector2.hpp"
#include "reference/referenceable.hpp"

namespace fro
{
   struct Collider final
   {
      Shape shape{ Circle{ 16.0 } };
      TransformMatrix transform{};
      double restitution{ 0.5 };
   };

   struct RigidBody final : Referenceable
   {
      Vector2<double> velocity{};
      std::vector<Collider> colliders{};
      double inverse_mass{ 1.0 };
   };
}

#endif