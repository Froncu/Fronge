#include "physics_system.hpp"
#include "ecs/group.hpp"
#include "utility/variant_visitor.hpp"

namespace fro
{
   std::optional<PhysicsSystem::Manifold> PhysicsSystem::are_colliding(Participant const& participant_a, Circle const& shape_a,
      Participant const& participant_b, Circle const& shape_b)
   {
      TransformMatrix const transform_matrix_a{
         participant_a.transform->world() * participant_a.rigid_body->colliders[participant_a.collider_index].transform
      };
      TransformMatrix const transform_matrix_b{
         participant_b.transform->world() * participant_b.rigid_body->colliders[participant_b.collider_index].transform
      };

      auto const [x_a, y_a]{ transform_matrix_a.scale() };
      auto const [x_b, y_b]{ transform_matrix_b.scale() };
      if ((x_a not_eq y_a or x_b not_eq y_b) and
         (x_a / y_a not_eq x_b / y_b or transform_matrix_a.rotation() not_eq transform_matrix_b.rotation()))
      {
         Locator::get<Logger>().warning_once(
            "attempted to solve collision between two circles with different rotations and non-uniform scale ratios; those will be skipped");
         return std::nullopt;
      }

      TransformMatrix const transform_matrix{ transform_matrix_a.inversed() * transform_matrix_b };
      Vector2 const delta{ transform_matrix.translation() };

      double const distance_squared{ delta.magnitude_squared() };
      double const total_radius{ shape_a.radius + transform_matrix.scale().x * shape_b.radius };
      if (distance_squared > total_radius * total_radius)
         return std::nullopt;

      Vector3<double> penetration;
      penetration.z = 0.0;
      if (not distance_squared)
      {
         penetration.x = 1.0;
         penetration.y = 0.0;
      }
      else
      {
         double const distance{ std::sqrt(distance_squared) };
         double const penetration_depth{ total_radius - distance };
         double const scalar{ penetration_depth / distance };
         penetration.x = delta.x * scalar;
         penetration.y = delta.y * scalar;
      }

      penetration = transform_matrix_a.transformation() * penetration;
      double const penetration_depth{ penetration.magnitude() };
      double const inverse_penetration_depth{ 1.0 / penetration_depth };
      Vector2 const penetration_normal{
         penetration.x * inverse_penetration_depth,
         penetration.y * inverse_penetration_depth
      };

      return Manifold{
         .contact_point{ transform_matrix_a.translation() + penetration_normal * shape_a.radius },
         .penetration_normal{ penetration_normal },
         .penetration_depth{ penetration_depth },
         .participant_a{ participant_a },
         .participant_b{ participant_b }
      };
   }

   void PhysicsSystem::resolve_collisions(Manifold const& manifold)
   {
      auto const& [rigid_body_a, transform_a, collider_index_a]{ manifold.participant_a };
      auto const& [rigid_body_b, transform_b, collider_index_b]{ manifold.participant_b };

      Vector2 const ra{ manifold.contact_point - transform_a->world().translation() };
      Vector2 const rb{ manifold.contact_point - transform_b->world().translation() };

      Vector2 relative_velocity{
         rigid_body_b->velocity + cross(rigid_body_b->angular_velocity, rb) -
         rigid_body_a->velocity - cross(rigid_body_a->angular_velocity, ra)
      };

      double velocity_along_normal{ relative_velocity * manifold.penetration_normal };
      if (velocity_along_normal > 0.0)
         return;

      double const restitution_coefficient{
         std::min(rigid_body_a->colliders[collider_index_a].restitution, rigid_body_b->colliders[collider_index_b].restitution)
      };

      double const ra_cross_n{ ra.cross(manifold.penetration_normal) };
      double const rb_cross_n{ rb.cross(manifold.penetration_normal) };
      double const inverse_mass_sum{
         rigid_body_a->inverse_mass +
         rigid_body_b->inverse_mass +
         ra_cross_n * ra_cross_n * rigid_body_a->inverse_inertia +
         rb_cross_n * rb_cross_n * rigid_body_b->inverse_inertia
      };

      double const impulse_scalar{ -(1 + restitution_coefficient) * velocity_along_normal / inverse_mass_sum };
      Vector2 const impulse{ manifold.penetration_normal * impulse_scalar };

      rigid_body_a->velocity -= impulse * rigid_body_a->inverse_mass;
      rigid_body_b->velocity += impulse * rigid_body_b->inverse_mass;

      rigid_body_a->angular_velocity -= ra.cross(impulse) * rigid_body_a->inverse_inertia;
      rigid_body_b->angular_velocity += rb.cross(impulse) * rigid_body_b->inverse_inertia;

      relative_velocity =
         rigid_body_b->velocity + cross(rigid_body_b->angular_velocity, rb) -
         rigid_body_a->velocity - cross(rigid_body_a->angular_velocity, ra);

      velocity_along_normal = relative_velocity * manifold.penetration_normal;

      Vector2 tangent{ relative_velocity - manifold.penetration_normal * velocity_along_normal };
      if (double const tangent_magnitude_squared{ tangent.magnitude_squared() }; tangent_magnitude_squared > 0)
         tangent /= std::sqrt(tangent_magnitude_squared);

      double const blended_friction{
         std::sqrt(
            rigid_body_a->colliders[collider_index_a].friction *
            rigid_body_b->colliders[collider_index_b].friction
         )
      };

      double const max_friction_impulse{ impulse_scalar * blended_friction };

      double friction_impulse_scalar{ -relative_velocity * tangent / inverse_mass_sum };
      friction_impulse_scalar = std::clamp(friction_impulse_scalar, -max_friction_impulse, max_friction_impulse);

      Vector2 const friction_impulse{ tangent * friction_impulse_scalar };

      rigid_body_a->velocity -= friction_impulse * rigid_body_a->inverse_mass;
      rigid_body_b->velocity += friction_impulse * rigid_body_b->inverse_mass;

      rigid_body_a->angular_velocity -= ra.cross(friction_impulse) * rigid_body_a->inverse_inertia;
      rigid_body_b->angular_velocity += rb.cross(friction_impulse) * rigid_body_b->inverse_inertia;
   }

   void PhysicsSystem::step(Scene const& scene, double const delta_seconds)
   {
      manifolds_.clear();

      auto& groups{ scene.group<Pack<RigidBody>, Pack<Transform>>() };

      for (auto&& [entity, rigid_body, transform] : groups)
      {
         if (rigid_body.inverse_mass)
            rigid_body.velocity += gravity * delta_seconds;

         transform.world_translate(rigid_body.velocity * delta_seconds);
         transform.world_rotate(rigid_body.angular_velocity * delta_seconds);
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
         for (Collider const& collider : rigid_body.colliders)
            render_context_->render(collider.shape, transform.world() * collider.transform);

      for (Manifold const& manifold : manifolds_)
         render_context_->render(Polygon{
            { manifold.contact_point, manifold.contact_point + manifold.penetration_normal * 8.0 }
         });
   }

   void PhysicsSystem::change_positional_correction_percent(double const percent)
   {
      positional_correction_percent_ = std::clamp(percent, 0.0, 1.0);
   }

   void PhysicsSystem::change_positional_correction_slop(double const slop)
   {
      positional_correction_slop_ = std::max(slop, 0.0);
   }

   std::optional<PhysicsSystem::Manifold> PhysicsSystem::are_colliding(Participant const& participant_a, Circle const& shape_a,
      Participant const& participant_b, Polygon const& shape_b)
   {
      TransformMatrix const transform_matrix_a{
         participant_a.transform->world() * participant_a.rigid_body->colliders[participant_a.collider_index].transform
      };
      TransformMatrix const transform_matrix_b{
         participant_b.transform->world() * participant_b.rigid_body->colliders[participant_b.collider_index].transform
      };

      TransformMatrix const transform_matrix{ transform_matrix_a.inversed() * transform_matrix_b };
      for (auto& [x, y] : transformed_polygon_.vertices = shape_b.vertices)
      {
         Vector3 const transformed_vertex{ transform_matrix.transformation() * Vector3{ x, y, 1.0 } };
         x = transformed_vertex.x;
         y = transformed_vertex.y;
      }

      Vector3<double> penetration_normal;
      penetration_normal.z = 0.0;
      double penetration_depth{ std::numeric_limits<double>::max() };

      enum class OverlapResult
      {
         NO_OVERLAP,
         OVERLAP,
         OVERLAP_WITH_SMALLER_PENETRATION
      };

      auto const query_overlap{
         [this, &shape_a, &penetration_normal, &penetration_depth] [[nodiscard]] (Vector2<double> const& axis)
         {
            auto const [min, max]{ transformed_polygon_.project(axis) };
            if (shape_a.radius < min or max < -shape_a.radius)
               return OverlapResult::NO_OVERLAP;

            double const test_penetration_depth{
               std::min(max, shape_a.radius) - std::max(min, -shape_a.radius)
            };

            if (test_penetration_depth >= penetration_depth)
               return OverlapResult::OVERLAP;

            penetration_normal.x = axis.x;
            penetration_normal.y = axis.y;
            penetration_depth = test_penetration_depth;

            return OverlapResult::OVERLAP_WITH_SMALLER_PENETRATION;
         }
      };

      for (std::size_t index{}; index < transformed_polygon_.vertices.size(); ++index)
      {
         Vector2<double> const& a{ transformed_polygon_.vertices[index] };
         Vector2<double> const& b{ transformed_polygon_.vertices[(index + 1) % transformed_polygon_.vertices.size()] };
         if (query_overlap((a - b).perpendicular().normalized()) == OverlapResult::NO_OVERLAP)
            return std::nullopt;
      }

      Vector2 closest_vertex{ transformed_polygon_.vertices.front() };
      double closest_vertex_distance_squared{ closest_vertex.magnitude_squared() };
      for (Vector2<double> const& vertex : std::views::drop(transformed_polygon_.vertices, 1))
         if (double const distance_squared{ vertex.magnitude_squared() }; distance_squared < closest_vertex_distance_squared)
         {
            closest_vertex = vertex;
            closest_vertex_distance_squared = distance_squared;
         }

      Vector3<double> contact_point;
      contact_point.z = 1.0;
      switch (query_overlap(closest_vertex / std::sqrt(closest_vertex_distance_squared)))
      {
         case OverlapResult::NO_OVERLAP:
            return std::nullopt;

         case OverlapResult::OVERLAP:
            contact_point.x = penetration_normal.x * shape_a.radius;
            contact_point.y = penetration_normal.y * shape_a.radius;
            break;

         case OverlapResult::OVERLAP_WITH_SMALLER_PENETRATION:
            contact_point.x = closest_vertex.x;
            contact_point.y = closest_vertex.y;
            break;
      }

      contact_point = transform_matrix_a.transformation() * contact_point;
      penetration_normal =
         transform_matrix_a.transformation().inversed().transposed() * (penetration_normal * penetration_depth);
      penetration_normal.z = 0.0;
      penetration_depth = penetration_normal.magnitude();
      double const inverse_penetration_depth{ 1.0 / penetration_depth };

      return Manifold{
         .contact_point{ contact_point.x, contact_point.y },
         .penetration_normal{
            penetration_normal.x * inverse_penetration_depth,
            penetration_normal.y * inverse_penetration_depth
         },
         .penetration_depth{ penetration_depth },
         .participant_a{ participant_a },
         .participant_b{ participant_b }
      };
   }

   // TODO: find the contact point
   std::optional<PhysicsSystem::Manifold> PhysicsSystem::are_colliding(Participant const& participant_a, Polygon const& shape_a,
      Participant const& participant_b, Polygon const& shape_b)
   {
      TransformMatrix const transform_matrix_a{
         participant_a.transform->world() * participant_a.rigid_body->colliders[participant_a.collider_index].transform
      };
      TransformMatrix const transform_matrix_b{
         participant_b.transform->world() * participant_b.rigid_body->colliders[participant_b.collider_index].transform
      };

      TransformMatrix const transform_matrix{ transform_matrix_a.inversed() * transform_matrix_b };
      for (auto& [x, y] : transformed_polygon_.vertices = shape_b.vertices)
      {
         Vector3 const transformed_vertex{ transform_matrix.transformation() * Vector3{ x, y, 1.0 } };
         x = transformed_vertex.x;
         y = transformed_vertex.y;
      }

      Vector3<double> penetration_normal;
      penetration_normal.z = 0.0;
      double penetration_depth{ std::numeric_limits<double>::max() };

      std::array<Polygon const*, 2> const polygons{ &shape_a, &transformed_polygon_ };
      for (std::size_t polygon_index{}; polygon_index < polygons.size(); ++polygon_index)
      {
         int const normal_scalar{ not polygon_index ? 1 : -1 };

         auto const& [vertices]{ *polygons[polygon_index] };
         for (std::size_t vertex_index{}; vertex_index < vertices.size(); ++vertex_index)
         {
            Vector2<double> const& a{ vertices[vertex_index] };
            Vector2<double> const& b{ vertices[(vertex_index + 1) % vertices.size()] };

            Vector2 const edge_normal{ (b - a).perpendicular().normalized() };

            auto&& [min_a, max_a]{ polygons[0]->project(edge_normal) };
            auto&& [min_b, max_b]{ polygons[1]->project(edge_normal) };
            if (max_a < min_b or max_b < min_a)
               return std::nullopt;

            if (double const test_penetration_depth{
               std::min(max_a, max_b) - std::max(min_a, min_b)
            }; test_penetration_depth < penetration_depth)
            {
               penetration_normal.x = edge_normal.x * normal_scalar;
               penetration_normal.y = edge_normal.y * normal_scalar;
               penetration_depth = test_penetration_depth;
            }
         }
      }

      penetration_normal = transform_matrix_a.transformation() * penetration_normal;
      penetration_depth = penetration_normal.magnitude();
      double const inverse_penetration_depth{ 1.0 / penetration_depth };

      return Manifold{
         .contact_point{},
         .penetration_normal{
            penetration_normal.x * inverse_penetration_depth,
            penetration_normal.y * inverse_penetration_depth
         },
         .penetration_depth{ penetration_depth },
         .participant_a{ participant_a },
         .participant_b{ participant_b }
      };
   }

   void PhysicsSystem::generate_manifolds(RigidBody& rigid_body_a, Transform& transform_a,
      RigidBody& rigid_body_b, Transform& transform_b)
   {
      for (std::size_t collider_index_a{}; collider_index_a < rigid_body_a.colliders.size(); ++collider_index_a)
         for (std::size_t collider_index_b{}; collider_index_b < rigid_body_b.colliders.size(); ++collider_index_b)
         {
            Participant const participant_a{
               .rigid_body{ Reference{ rigid_body_a } },
               .transform{ Reference{ transform_a } },
               .collider_index{ collider_index_a }
            };

            Participant const participant_b{
               .rigid_body{ Reference{ rigid_body_b } },
               .transform{ Reference{ transform_b } },
               .collider_index{ collider_index_b }
            };

            VariantVisitor overlap_checker
            {
               [&participant_a, &participant_b](Circle const& circle_a, Circle const& circle_b)
               {
                  return are_colliding(participant_a, circle_a, participant_b, circle_b);
               },

               [this, &participant_a, &participant_b](Polygon const& polygon, Circle const& circle)
               {
                  return are_colliding(participant_b, circle, participant_a, polygon);
               },

               [this, &participant_a, &participant_b](Circle const& circle, Polygon const& polygon)
               {
                  return are_colliding(participant_a, circle, participant_b, polygon);
               },

               [this, &participant_a, &participant_b](Polygon const& polygon_a, Polygon const& polygon_b)
               {
                  return are_colliding(participant_a, polygon_a, participant_b, polygon_b);
               }
            };

            std::optional manifold{
               overlap_checker(rigid_body_a.colliders[collider_index_a].shape, rigid_body_b.colliders[collider_index_b].shape)
            };

            if (not manifold.has_value())
               continue;

            manifolds_.emplace_back(std::move(manifold.value()));
         }
   }

   void PhysicsSystem::positional_correction(Manifold const& manifold) const
   {
      Vector2 const correction{
         manifold.penetration_normal *
         positional_correction_percent_ *
         std::max(manifold.penetration_depth - positional_correction_slop_, 0.0) /
         (manifold.participant_a.rigid_body->inverse_mass + manifold.participant_b.rigid_body->inverse_mass)
      };

      manifold.participant_a.transform->world_translate(correction * -manifold.participant_a.rigid_body->inverse_mass);
      manifold.participant_b.transform->world_translate(correction * manifold.participant_b.rigid_body->inverse_mass);
   }
}