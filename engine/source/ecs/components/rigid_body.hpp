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
      TransformMatrix transform;
      Shape shape;
      double static_friction;
      double dynamic_friction;
      double restitution;
   };

   struct RigidBody final : Referenceable
   {
      void apply_force(Vector2<double> const& f)
      {
         force += f;
      }

      void apply_impulse(Vector2<double> const& impulse, Vector2<double> const& contact_vector)
      {
         velocity += impulse * inverse_mass;
         angular_velocity += inverse_inertia * contact_vector.cross(impulse);
      }

      Vector2<double> velocity;
      Vector2<double> force;
      double angular_velocity;
      double torque;

      double inertia;
      double inverse_inertia;
      double mass;
      double inverse_mass;

      Collider collider;
   };
}

#endif