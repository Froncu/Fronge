#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "maths/transform_matrix.hpp"
#include "maths/vector2.hpp"

namespace fro
{
   struct Collider final
   {
      TransformMatrix transform;
      // Shape shape;
      double static_friction;
      double dynamic_friction;
      double restitution;
   };

   struct RigidBody final
   {
      Vector2<double> velocity;
      Vector2<double> force;
      double angular_velocity;
      double torque;

      double inertia;
      double inverse_inertia;
      double mass;
      double inverse_mass;

      std::vector<Collider> colliders;
   };
}

#endif