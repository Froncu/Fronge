#include "ecs/components.hpp"
#include "ecs/group.hpp"
#include "physics_system.hpp"

namespace fro
{
   void PhysicsSystem::Manifold::solve()
   {
   }

   void PhysicsSystem::Manifold::initialize(double const fixed_delta_seconds, Vector2<double> const gravity)
   {
      // Calculate average restitution
      e = std::min(rigid_body_a->collider.restitution, rigid_body_b->collider.restitution);

      // Calculate static and dynamic friction
      sf = std::sqrt(rigid_body_a->collider.static_friction * rigid_body_a->collider.static_friction);
      df = std::sqrt(rigid_body_a->collider.dynamic_friction * rigid_body_a->collider.dynamic_friction);

      for (std::uint32_t i = 0; i < contact_count; ++i)
      {
         // Calculate radii from COM to contact
         Vector2<double> ra = contacts[i] - transform_a->world().translation();
         Vector2<double> rb = contacts[i] - transform_b->world().translation();

         Vector2<double> rv =
            rigid_body_b->velocity + cross(rigid_body_b->angular_velocity, rb) -
            rigid_body_a->velocity - cross(rigid_body_a->angular_velocity, ra);

         // Determine if we should perform a resting collision or not
         // The idea is if the only thing moving this object is gravity,
         // then the collision should be performed without any restitution
         if (rv.magnitude_squared() < (gravity * fixed_delta_seconds).magnitude_squared() + 0.0001)
            e = 0.0f;
      }
   }

   void PhysicsSystem::Manifold::apply_impulse()
   {
      // Early out and positional correct if both objects have infinite mass
      if (std::abs(rigid_body_a->inverse_mass + rigid_body_b->inverse_mass) <= 0.0001)
      {
         infinite_mass_correction();
         return;
      }

      for (std::uint32_t i = 0; i < contact_count; ++i)
      {
         // Calculate radii from COM to contact
         Vector2<double> ra = contacts[i] - transform_a->world().translation();
         Vector2<double> rb = contacts[i] - transform_b->world().translation();

         // Relative velocity
         Vector2<double> rv =
            rigid_body_b->velocity + cross(rigid_body_b->angular_velocity, rb) -
            rigid_body_a->velocity - cross(rigid_body_a->angular_velocity, ra);

         // Relative velocity along the normal
         double const contact_vel = rv * normal;

         // Do not resolve if velocities are separating
         if (contact_vel > 0)
            return;

         double const ra_cross_n = ra.cross(normal);
         double const rb_cross_n = rb.cross(normal);
         double const inv_mass_sum =
            rigid_body_a->inverse_mass + rigid_body_b->inverse_mass + std::sqrt(ra_cross_n) *
            rigid_body_a->inverse_inertia + std::sqrt(rb_cross_n) * rigid_body_b->inverse_inertia;

         // Calculate impulse scalar
         double j = -(1.0f + e) * contact_vel;
         j /= inv_mass_sum;
         j /= static_cast<double>(contact_count);

         // Apply impulse
         Vector2<double> impulse = normal * j;
         rigid_body_a->apply_impulse(-impulse, ra);
         rigid_body_b->apply_impulse(impulse, rb);

         // Friction impulse
         rv =
            rigid_body_b->velocity + cross(rigid_body_b->angular_velocity, rb) -
            rigid_body_a->velocity - cross(rigid_body_a->angular_velocity, ra);

         Vector2<double> t = rv - normal * (rv * normal);
         t.normalize();

         // j tangent magnitude
         double jt = -(rv * t);
         jt /= inv_mass_sum;
         jt /= static_cast<double>(contact_count);

         // Don't apply tiny friction impulses
         if (jt < 0.0001)
            return;

         // Coulumb's law
         Vector2<double> tangent_impulse;
         if (std::abs(jt) < j * sf)
            tangent_impulse = t * jt;
         else
            tangent_impulse = t * -j * df;

         // Apply friction impulse
         rigid_body_a->apply_impulse(-tangent_impulse, ra);
         rigid_body_b->apply_impulse(tangent_impulse, rb);
      }
   }

   void PhysicsSystem::Manifold::positional_correction()
   {
      double const k_slop = 0.05f; // Penetration allowance
      double const percent = 0.4f; // Penetration percentage to correct
      Vector2<double> correction =
         normal * (std::max(penetration - k_slop, 0.0) / (rigid_body_a->inverse_mass + rigid_body_b->inverse_mass)) * percent;

      transform_a->world().translation() -= correction * rigid_body_a->inverse_mass;
      transform_b->world().translation() += correction * rigid_body_b->inverse_mass;
   }

   void PhysicsSystem::Manifold::infinite_mass_correction() const
   {
      rigid_body_a->velocity.x = 0.0;
      rigid_body_a->velocity.y = 0.0;
      rigid_body_b->velocity.x = 0.0;
      rigid_body_b->velocity.y = 0.0;
   }

   void PhysicsSystem::step(Scene const& scene, double fixed_delta_seconds)
   {
      manifolds_.clear();

      auto& groups{ scene.group<Pack<RigidBody>, Pack<Transform>>() };
      for (auto group_a{ groups.begin() }; group_a not_eq groups.end(); ++group_a)
      {
         auto&& [entity_a, rigid_body_a, transform_a]{ *group_a };
         for (auto group_b{ std::next(group_a) }; group_b not_eq groups.end(); ++group_b)
         {
            auto&& [entity_b, rigid_body_b, transform_b]{ *group_b };

            if (rigid_body_a.inverse_mass == 0 and rigid_body_b.inverse_mass == 0)
               continue;

            Manifold manifold{ transform_a, rigid_body_a, transform_b, rigid_body_b };
            manifold.solve();
            if (manifold.contact_count)
               manifolds_.emplace_back(std::move(manifold));
         }
      }


   }
}