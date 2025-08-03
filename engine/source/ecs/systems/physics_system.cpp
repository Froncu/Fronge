#include "physics_system.hpp"
#include "ecs/group.hpp"
#include "utility/variant_visitor.hpp"

namespace fro
{
   void PhysicsSystem::resolve_collisions(Manifold const& manifold)
   {
      auto const& [rigid_body_a, transform_a]{ manifold.participant_a };
      auto const& [rigid_body_b, transform_b]{ manifold.participant_b };

      Vector2 const relative_velocity{ rigid_body_b->velocity - rigid_body_a->velocity };
      double const velocity_along_normal{ relative_velocity * manifold.normal };

      if (velocity_along_normal > 0.0)
         return;

      double const restitution_coefficient{ std::min(rigid_body_a->restitution, rigid_body_b->restitution) };
      double const impulse_scalar{
         -(1 + restitution_coefficient) * velocity_along_normal / (rigid_body_a->inverse_mass + rigid_body_b->inverse_mass)
      };

      Vector2 const impulse{ manifold.normal * impulse_scalar };
      rigid_body_a->velocity -= impulse * rigid_body_a->inverse_mass;
      rigid_body_b->velocity += impulse * rigid_body_b->inverse_mass;
   }

   void PhysicsSystem::step(Scene const& scene, double const delta_seconds)
   {
      manifolds_.clear();

      auto& groups{ scene.group<Pack<RigidBody>, Pack<Transform>>() };

      for (auto&& [entity, rigid_body, transform] : groups)
      {
         rigid_body.velocity += gravity * delta_seconds;
         transform.world_translate(rigid_body.velocity * delta_seconds);
      }

      for (auto group_a{ groups.begin() }; group_a not_eq groups.end(); ++group_a)
      {
         auto&& [entity_a, rigid_body_a, transform_a]{ *group_a };
         for (auto group_b{ std::next(group_a) }; group_b not_eq groups.end(); ++group_b)
         {
            auto&& [entity_b, rigid_body_b, transform_b]{ *group_b };
            generate_manifolds(rigid_body_a, transform_a, rigid_body_b, transform_b);
         }
      }

      for (Manifold const& manifold : manifolds_)
         resolve_collisions(manifold);

      for (Manifold const& manifold : manifolds_)
         positional_correction(manifold);
   }

   void PhysicsSystem::render(Scene const& scene) const
   {
      for (auto& groups{ scene.group<Pack<RigidBody>, Pack<Transform>>() };
           auto const& [entity, rigid_body, transform] : groups)
         for (auto const& [shape, translation] : rigid_body.colliders)
            render_context_->render(shape, transform.world().translated(translation));
   }

   void PhysicsSystem::change_positional_correction_percent(double const percent)
   {
      positional_correction_percent_ = std::clamp(percent, 0.0, 1.0);
   }

   void PhysicsSystem::change_positional_correction_slop(double const slop)
   {
      positional_correction_slop_ = std::max(slop, 0.0);
   }

   void PhysicsSystem::generate_manifolds(RigidBody& rigid_body_a, Transform& transform_a,
      RigidBody& rigid_body_b, Transform& transform_b)
   {
      for (auto collider_a{ rigid_body_a.colliders.begin() }; collider_a not_eq rigid_body_a.colliders.end(); ++collider_a)
         for (auto collider_b{ rigid_body_b.colliders.begin() }; collider_b not_eq rigid_body_b.colliders.end(); ++collider_b)
         {
            VariantVisitor overlap_checker{
               [&rigid_body_a, &transform_a, collider_a, &rigid_body_b, &transform_b, collider_b]
            (Circle const& shape_a, Circle const& shape_b) -> std::optional<Manifold>
               {
                  Vector2 const position_a{ transform_a.world().translation() + collider_a->translation };
                  Vector2 const position_b{ transform_b.world().translation() + collider_b->translation };
                  Vector2 const delta{ position_b - position_a };

                  double const distance_squared{ delta.magnitude_squared() };
                  double const total_radius{ shape_a.radius + shape_b.radius };
                  if (distance_squared > total_radius * total_radius)
                     return std::nullopt;

                  Vector2<double> normal;
                  double penetration;

                  if (not distance_squared)
                  {
                     normal.x = 1.0;
                     normal.y = 0.0;
                     penetration = shape_a.radius;
                  }
                  else
                  {
                     double const distance{ std::sqrt(distance_squared) };
                     normal = delta / distance;
                     penetration = total_radius - distance;
                  }

                  return Manifold{
                     .normal{ normal },
                     .penetration{ penetration },
                     .participant_a{ Reference{ rigid_body_a }, Reference{ transform_a } },
                     .participant_b{ Reference{ rigid_body_b }, Reference{ transform_b } }
                  };
               },

               // TODO implement other shape checks
               [](Circle const&, Polygon const&) -> std::optional<Manifold>
               {
                  return std::nullopt;
               },

               [](Polygon const&, Circle const&) -> std::optional<Manifold>
               {
                  return std::nullopt;
               },

               [](Polygon const&, Polygon const&) -> std::optional<Manifold>
               {
                  return std::nullopt;
               }
            };

            std::optional manifold{ overlap_checker(collider_a->shape, collider_b->shape) };
            if (not manifold.has_value())
               continue;

            manifolds_.emplace_back(std::move(manifold.value()));
         }
   }

   void PhysicsSystem::positional_correction(Manifold const& manifold) const
   {
      Vector2 const correction{
         manifold.normal * positional_correction_percent_ * std::max(manifold.penetration - positional_correction_slop_, 0.0) /
         (manifold.participant_a.rigid_body->inverse_mass + manifold.participant_b.rigid_body->inverse_mass)
      };

      manifold.participant_a.transform->world_translate(correction * -manifold.participant_a.rigid_body->inverse_mass);
      manifold.participant_b.transform->world_translate(correction * manifold.participant_b.rigid_body->inverse_mass);
   }
}