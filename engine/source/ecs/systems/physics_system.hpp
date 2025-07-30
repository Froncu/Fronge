#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "ecs/scene.hpp"

namespace fro
{
   class PhysicsSystem final
   {
      struct Manifold
      {
         Manifold(Transform& transform_a, RigidBody& rigid_body_a, Transform& transform_b, RigidBody& rigid_body_b)
            : transform_a{ transform_a }
            , rigid_body_a{ rigid_body_a }
            , transform_b{ transform_b }
            , rigid_body_b{ rigid_body_b }
         {
         }

         void solve(); // Generate contact information
         void initialize(double fixed_delta_seconds, Vector2<double> gravity); // Precalculations for impulse solving
         void apply_impulse(); // Solve impulse and apply
         void positional_correction(); // Naive correction of positional penetration
         void infinite_mass_correction() const;

         Reference<Transform> transform_a;
         Reference<RigidBody> rigid_body_a;
         Reference<Transform> transform_b;
         Reference<RigidBody> rigid_body_b;

         double penetration{}; // Depth of penetration from collision
         Vector2<double> normal{}; // From A to B
         std::array<Vector2<double>, 2> contacts{}; // Points of contact during collision
         std::uint32_t contact_count{}; // Number of contacts that occured during collision
         double e{}; // Mixed restitution
         double df{}; // Mixed dynamic friction
         double sf{}; // Mixed static friction
      };

      public:
         PhysicsSystem() = default;
         PhysicsSystem(PhysicsSystem const&) = default;
         PhysicsSystem(PhysicsSystem&&) noexcept = default;

         ~PhysicsSystem() = default;

         PhysicsSystem& operator=(PhysicsSystem const&) = default;
         PhysicsSystem& operator=(PhysicsSystem&&) noexcept = default;

         void step(Scene const& scene, double fixed_delta_seconds);

         Vector2<double> gravity{ 0.0, 50.0 };

      private:
         std::vector<Manifold> manifolds_{};
   };
}

#endif