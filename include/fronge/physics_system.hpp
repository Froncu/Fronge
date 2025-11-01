#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "fronge/locator.hpp"
#include "fronge/renderer.hpp"
#include "fronge/scene.hpp"

namespace fro
{
   class PhysicsSystem final
   {
      struct Participant final
      {
         Reference<RigidBody> const rigid_body;
         Reference<Transform> const transform;
         std::size_t const collider_index;
      };

      struct Manifold final
      {
         Vector2<double> contact_point;
         Vector2<double> penetration_normal;
         double penetration_depth;
         Participant participant_a;
         Participant participant_b;
      };

      public:
         PhysicsSystem() = default;
         PhysicsSystem(PhysicsSystem const&) = default;
         PhysicsSystem(PhysicsSystem&&) noexcept = default;

         ~PhysicsSystem() = default;

         PhysicsSystem& operator=(PhysicsSystem const&) = default;
         PhysicsSystem& operator=(PhysicsSystem&&) noexcept = default;

         FRO_API void step(Scene const& scene, double delta_seconds);
         FRO_API void render(Scene const& scene) const;

         FRO_API void change_positional_correction_percent(double percent);
         FRO_API void change_positional_correction_slop(double slop);

         Vector2<double> gravity{ 0.0, 9.81 };

      private:
         [[nodiscard]] static std::optional<Manifold> are_colliding(Participant const& participant_a, Circle const& shape_a,
            Participant const& participant_b, Circle const& shape_b);
         static void resolve_collisions(Manifold const& manifold);

         [[nodiscard]] std::optional<Manifold> are_colliding(Participant const& participant_a, Circle const& shape_a,
            Participant const& participant_b, Polygon const& shape_b);
         [[nodiscard]] std::optional<Manifold> are_colliding(Participant const& participant_a, Polygon const& shape_a,
            Participant const& participant_b, Polygon const& shape_b);
         void generate_manifolds(RigidBody& rigid_body_a, Transform& transform_a,
            RigidBody& rigid_body_b, Transform& transform_b);
         void positional_correction(Manifold const& manifold) const;

         std::vector<Manifold> manifolds_{};
         Polygon transformed_polygon_{};
         Reference<Renderer> renderer_{ Locator::get<Renderer>() };
         double positional_correction_percent_{ 0.2 };
         double positional_correction_slop_{ 0.01 };
   };
}

#endif