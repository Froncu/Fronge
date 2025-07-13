#include "physics_system.hpp"

namespace fro
{
   bool PhysicsSystem::aabb_vs_aabb(AABB const& a, AABB const& b) const
   {
      if (a.max.x < b.min.x or a.min.x > b.max.x)
         return false;

      if (a.max.y < b.min.y or a.min.y > b.max.y)
         return false;

      return true;
   }

   bool PhysicsSystem::circle_vs_circle(Circle const& a, Circle const& b) const
   {
      double const max_distance_squared{ std::pow(a.radius + b.radius, 2) };
      double const distance_squared{ std::pow(a.position.x - b.position.x, 2) + std::pow(a.position.y - b.position.y, 2) };

      return distance_squared <= max_distance_squared;
   }

   void PhysicsSystem::resolve_collision(Object& a, Object& b)
   {
      Vector2<double> constexpr collision_normal{};

      Vector2 const relative_velocity{ b.velocity - a.velocity };
      double const velocity_along_normal{ relative_velocity * collision_normal };
      if (velocity_along_normal > 0)
         return;

      double const restitution{ std::min(a.restitution, b.restitution) };
      double impulse_mangitude{ -(1 + restitution) * velocity_along_normal };
      impulse_mangitude /= a.inverse_mass + b.inverse_mass;

      Vector2 const impulse{ collision_normal * impulse_mangitude };
      a.velocity -= impulse * a.inverse_mass;
      b.velocity += impulse * b.inverse_mass;
   }

   void PhysicsSystem::positional_correction(Object& a, Object& b)
   {
      Vector2<double> constexpr collision_normal{};
      float constexpr penetration{};

      float constexpr penetration_threshold{ 0.01 };
      float constexpr percent{ 0.2 }; // usually 20% to 80%

      if (penetration < penetration_threshold)
         return;

      Vector2 const correction{ collision_normal * (penetration / (a.inverse_mass + b.inverse_mass) * percent) };
      a.position -= correction * a.inverse_mass;
      b.position += correction * b.inverse_mass;
   }
}