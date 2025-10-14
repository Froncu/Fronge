#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "base_component.hpp"
#include "maths/shapes.hpp"
#include "maths/transform_matrix/transform_matrix.hpp"
#include "maths/vector2.hpp"

namespace fro
{
   struct Collider final
   {
      Shape shape{ Circle{ 16.0 } };
      TransformMatrix transform{};
      double restitution{ 0.5 };
      double friction{ 0.5 };
   };

   struct RigidBody final : BaseComponent
   {
      std::vector<Collider> colliders{};

      Vector2<double> velocity{};
      double angular_velocity{};

      double inverse_inertia{};
      double inverse_mass{ 1.0 };
   };
}

#endif