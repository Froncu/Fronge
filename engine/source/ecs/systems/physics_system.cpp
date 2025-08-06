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
         Locator::get<Logger>().error(
            "attempted to solve collision between two circles with different rotations and non-uniform scale ratios; those will be skipped");
         return std::nullopt;
      }

      TransformMatrix const transform_matrix{ transform_matrix_a.inversed() * transform_matrix_b };
      Vector2 const delta{ transform_matrix.translation() };

      double const distance_squared{ delta.magnitude_squared() };
      double const total_radius{ shape_a.radius + transform_matrix.scale().x * shape_b.radius };
      if (distance_squared > total_radius * total_radius)
         return std::nullopt;

      Vector2<double> penetration_normal;
      double penetration_depth;
      if (not distance_squared)
      {
         penetration_normal.x = 1.0;
         penetration_normal.y = 0.0;
         penetration_depth = shape_a.radius;
      }
      else
      {
         double const distance{ std::sqrt(distance_squared) };
         penetration_normal = delta / distance;
         penetration_depth = total_radius - distance;
      }

      return Manifold{
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

      Vector2 const relative_velocity{ rigid_body_b->velocity - rigid_body_a->velocity };
      double const velocity_along_normal{ relative_velocity * manifold.penetration_normal };

      if (velocity_along_normal > 0.0)
         return;

      double const restitution_coefficient{
         std::min(rigid_body_a->colliders[collider_index_a].restitution, rigid_body_b->colliders[collider_index_b].restitution)
      };
      double const impulse_scalar{
         -(1 + restitution_coefficient) * velocity_along_normal / (rigid_body_a->inverse_mass + rigid_body_b->inverse_mass)
      };

      Vector2 const impulse{ manifold.penetration_normal * impulse_scalar };
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
         for (auto const& [shape, local_transform, restitution] : rigid_body.colliders)
            render_context_->render(shape, transform.world() * local_transform);
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

      Vector2<double> smallest_axis;
      double min_overlap{ std::numeric_limits<double>::max() };

      for (std::size_t index{}; index < transformed_polygon_.vertices.size(); ++index)
      {
         Vector2<double> const& a{ transformed_polygon_.vertices[index] };
         Vector2<double> const& b{ transformed_polygon_.vertices[(index + 1) % transformed_polygon_.vertices.size()] };

         Vector2 axis{ b - a };
         axis = axis.perpendicular().normalized();

         auto&& [min_polygon, max_polygon]{ transformed_polygon_.project(axis) };
         if (shape_a.radius < min_polygon or max_polygon < -shape_a.radius)
            return std::nullopt;

         if (double const overlap{
            std::min(max_polygon, shape_a.radius) - std::max(min_polygon, -shape_a.radius)
         }; overlap < min_overlap)
         {
            smallest_axis = axis;
            min_overlap = overlap;
         }
      }

      double min_distance{ std::numeric_limits<float>::max() };
      Vector2<double> closest_vertex;
      for (Vector2<double> const& vertex : transformed_polygon_.vertices)
         if (double const distance_squared{ vertex.magnitude_squared() }; distance_squared < min_distance)
         {
            min_distance = distance_squared;
            closest_vertex = vertex;
         }

      min_distance = std::sqrt(min_distance);
      Vector2 const axis{ closest_vertex / min_distance };

      auto&& [min_polygon, max_polygon]{ transformed_polygon_.project({ axis.x, axis.y }) };
      if (shape_a.radius < min_polygon or max_polygon < -shape_a.radius)
         return std::nullopt;

      if (double const overlap{
         std::min(max_polygon, shape_a.radius) - std::max(min_polygon, -shape_a.radius)
      }; overlap < min_overlap)
      {
         min_overlap = overlap;
         smallest_axis = axis;
      }

      return Manifold{
         .penetration_normal{ closest_vertex * smallest_axis < 0.0 ? -smallest_axis : smallest_axis },
         .penetration_depth{ min_overlap },
         .participant_a{ participant_a },
         .participant_b{ participant_b }
      };
   }

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

      double min_overlap{ std::numeric_limits<double>::max() };
      Vector2<double> smallest_axis;
      std::array<Polygon const*, 2> const polygons{ &shape_a, &transformed_polygon_ };

      for (auto& polygon : polygons)
         for (std::size_t index{}; index < polygon->vertices.size(); ++index)
         {
            Vector2<double> const& a{ polygon->vertices[index] };
            Vector2<double> const& b{ polygon->vertices[(index + 1) % polygon->vertices.size()] };

            Vector2 axis{ b - a };
            axis = axis.perpendicular().normalized();

            auto&& [min_a, max_a]{ polygons[0]->project(axis) };
            auto&& [min_b, max_b]{ polygons[1]->project(axis) };
            if (max_a < min_b or max_b < min_a)
               return std::nullopt;

            if (double const overlap{
               std::min(max_a, max_b) - std::max(min_a, min_b)
            }; overlap < min_overlap)
            {
               min_overlap = overlap;
               smallest_axis = axis;
            }
         }

      Vector2 const direction{ polygons[1]->center() - polygons[0]->center() };
      return Manifold{
         .penetration_normal{ direction * smallest_axis < 0.0 ? -smallest_axis : smallest_axis },
         .penetration_depth{ min_overlap },
         .participant_a{ participant_a },
         .participant_b{ participant_b }
      };
   }

   void PhysicsSystem::generate_manifolds(RigidBody& rigid_body_a, Transform& transform_a,
      RigidBody& rigid_body_b, Transform& transform_b)
   {
      for (int collider_index_a{}; collider_index_a < rigid_body_a.colliders.size(); ++collider_index_a)
         for (int collider_index_b{}; collider_index_b < rigid_body_b.colliders.size(); ++collider_index_b)
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