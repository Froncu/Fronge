#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "maths/vector2.hpp"

namespace fro
{
   struct AABB final
   {
      Vector2<double> min;
      Vector2<double> max;
   };

   struct Circle
   {
      double radius;
      Vector2<double> position;
   };

   struct Object final
   {
      Vector2<double> position;
      Vector2<double> velocity;
      double restitution;
      double inverse_mass;
   };

   class PhysicsSystem final
   {
      public:
         PhysicsSystem() = default;
         PhysicsSystem(PhysicsSystem const&) = default;
         PhysicsSystem(PhysicsSystem&&) noexcept = default;

         ~PhysicsSystem() = default;

         PhysicsSystem& operator=(PhysicsSystem const&) = default;
         PhysicsSystem& operator=(PhysicsSystem&&) noexcept = default;

      private:
         [[nodiscard]] bool aabb_vs_aabb(AABB const& a, AABB const& b) const;
         [[nodiscard]] bool circle_vs_circle(Circle const& a, Circle const& b) const;

         void resolve_collision(Object& a, Object& b);
         void positional_correction(Object& a, Object& b);
   };
}

#endif