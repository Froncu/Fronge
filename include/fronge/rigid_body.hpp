#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "fronge/shapes.hpp"
#include "fronge/transform_matrix.hpp"
#include "fronge/vector2.hpp"
#include "fronge/referenceable.hpp"

namespace fro
{
   struct Collider final
   {
      Shape shape{ Circle{ 16.0 } };
      TransformMatrix transform{};
      double restitution{ 0.5 };
      double friction{ 0.5 };
   };

   struct RigidBody final : Referenceable
   {
      std::vector<Collider> colliders{};

      Vector2<double> velocity{};
      double angular_velocity{};

      double inverse_inertia{};
      double inverse_mass{ 1.0 };
   };
}

#endif