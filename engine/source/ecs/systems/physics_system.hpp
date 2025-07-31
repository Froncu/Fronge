#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "ecs/scene.hpp"
#include "services/locator.hpp"
#include "services/render_context/render_context.hpp"

namespace fro
{
   class PhysicsSystem final
   {
      struct Participant final
      {
         Reference<RigidBody> rigid_body{};
         Reference<Transform> transform{};
      };

      struct Manifold final
      {
         Vector2<double> normal{};
         double penetration{};

         Participant participant_a{};
         Participant participant_b{};
      };

      public:
         PhysicsSystem() = default;
         PhysicsSystem(PhysicsSystem const&) = default;
         PhysicsSystem(PhysicsSystem&&) noexcept = default;

         ~PhysicsSystem() = default;

         PhysicsSystem& operator=(PhysicsSystem const&) = default;
         PhysicsSystem& operator=(PhysicsSystem&&) noexcept = default;

         void step(Scene const& scene, double delta_seconds);
         void render(Scene const& scene) const;

      private:
         static void resolve_collisions(Manifold const& manifold);
         static void positional_correction(Manifold const& manifold);
         void generate_manifolds(RigidBody& rigid_body_a, Transform& transform_a,
            RigidBody& rigid_body_b, Transform& transform_b);

         std::vector<Manifold> manifolds_{};
         Reference<RenderContext> render_context_{ Locator::get<RenderContext>() };
   };
}

#endif