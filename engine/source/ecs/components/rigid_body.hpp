#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "maths/shapes.hpp"
#include "maths/vector2.hpp"
#include "reference/referenceable.hpp"

namespace fro
{
   struct Collider final
   {
      Shape shape{};
      Vector2<double> translation{};
   };

   struct RigidBody final : Referenceable
   {
      Vector2<double> velocity{};
      std::vector<Collider> colliders{};
      double restitution{ 0.5 };
      double inverse_mass{ 1.0 };
   };
}

#endif