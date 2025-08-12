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
      double restitution{ 0.9 };
      double friction{ 0.8 };
   };

   struct RigidBody final : Referenceable
   {
      std::vector<Collider> colliders{};

      Vector2<double> velocity{};
      double angular_velocity{};

      double inverse_inertia{ 0.01 };
      double inverse_mass{ 1.0 };
   };
}

#endif