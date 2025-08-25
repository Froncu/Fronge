#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "core.hpp"
#include "maths/transform_matrix/transform_matrix.hpp"
#include "reference/reference.hpp"
#include "reference/referenceable.hpp"

namespace fro
{
   class Transform final : public Referenceable
   {
      public:
         Transform() = default;
         Transform(Transform const&) = default;
         Transform(Transform&&) noexcept = default;

         FRO_API virtual ~Transform() override;

         Transform& operator=(Transform const&) = default;
         Transform& operator=(Transform&&) noexcept = default;

         FRO_API bool change_parent(Transform* parent, bool keep_world_transform = true);

         FRO_API void change_local_translation(Vector2<double> translation);
         FRO_API void change_local_rotation(double rotation);
         FRO_API void change_local_scale(Vector2<double> scale);

         FRO_API void local_translate(Vector2<double> translation);
         FRO_API void local_rotate(double rotation);
         FRO_API void local_scale(Vector2<double> scale);

         FRO_API void change_world_translation(Vector2<double> translation);
         FRO_API void change_world_rotation(double rotation);
         FRO_API void change_world_scale(Vector2<double> scale);

         FRO_API void world_translate(Vector2<double> translation);
         FRO_API void world_rotate(double rotation);
         FRO_API void world_scale(Vector2<double> scale);

         FRO_API [[nodiscard]] TransformMatrix const& local() const;
         FRO_API [[nodiscard]] TransformMatrix const& world() const;

         FRO_API [[nodiscard]] Reference<Transform> parent() const;
         FRO_API [[nodiscard]] std::span<Reference<Transform> const> children() const;

         FRO_API [[nodiscard]] bool is_parenting(Transform const& transform) const;

      private:
         void mark_world_transform_dirty() const;
         void calculate_local_transform();
         void calculate_world_transform() const;

         [[nodiscard]] TransformMatrix& world_transform() const;

         TransformMatrix local_transform_{};
         mutable TransformMatrix world_transform_{};

         Reference<Transform> parent_{};
         std::vector<Reference<Transform>> children_{};

         mutable bool is_world_transform_dirty_{};
   };
}

#endif