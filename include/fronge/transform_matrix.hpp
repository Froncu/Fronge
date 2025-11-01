#ifndef TRANSFORM_MATRIX_HPP
#define TRANSFORM_MATRIX_HPP

#include "fronge/api.hpp"
#include "fronge/matrix.hpp"
#include "fronge/vector2.hpp"

namespace fro
{
   class TransformMatrix final
   {
      public:
         using Component = double;
         using Matrix = Matrix<Component>;

         TransformMatrix() = default;
         FRO_API explicit TransformMatrix(Matrix const& transformation);
         TransformMatrix(TransformMatrix const&) = default;
         TransformMatrix(TransformMatrix&&) noexcept = default;

         ~TransformMatrix() = default;

         TransformMatrix& operator=(TransformMatrix const&) = default;
         TransformMatrix& operator=(TransformMatrix&&) noexcept = default;
         FRO_API [[nodiscard]] Matrix::Row operator[](std::size_t index) const;
         FRO_API [[nodiscard]] Vector3<Component> operator*(Vector3<Component> const& vector) const;
         FRO_API [[nodiscard]] TransformMatrix operator*(TransformMatrix const& other) const;
         FRO_API TransformMatrix& operator*=(TransformMatrix const& other);
         FRO_API [[nodiscard]] bool operator==(TransformMatrix const& other) const;

         FRO_API TransformMatrix& change_translation(Vector2<Component> translation);
         FRO_API TransformMatrix& change_rotation(Component rotation_radians);
         FRO_API TransformMatrix& change_scale(Vector2<Component> scale);

         FRO_API [[nodiscard]] TransformMatrix changed_translation(Vector2<Component> translation) const;
         FRO_API [[nodiscard]] TransformMatrix changed_rotation(Component rotation_radians) const;
         FRO_API [[nodiscard]] TransformMatrix changed_scale(Vector2<Component> scale) const;

         FRO_API TransformMatrix& translate(Vector2<Component> translation);
         FRO_API TransformMatrix& rotate(Component rotation_radians);
         FRO_API TransformMatrix& scale(Vector2<Component> scale);

         FRO_API [[nodiscard]] TransformMatrix translated(Vector2<Component> translation) const;
         FRO_API [[nodiscard]] TransformMatrix rotated(Component rotation_radians) const;
         FRO_API [[nodiscard]] TransformMatrix scaled(Vector2<Component> scale) const;

         FRO_API [[nodiscard]] Vector2<Component> translation() const;
         FRO_API [[nodiscard]] Component rotation() const;
         FRO_API [[nodiscard]] Vector2<Component> scale() const;

         FRO_API [[nodiscard]] TransformMatrix transposed() const;
         FRO_API TransformMatrix& transpose();
         FRO_API [[nodiscard]] TransformMatrix inversed() const;
         FRO_API TransformMatrix& inverse();

      private:
         [[nodiscard]] Matrix const& transformation() const;
         void calculate_rotation() const;
         void calculate_scale() const;

         mutable Matrix transformation_{ true };

         mutable Vector2<Component> scale_{ 1, 1 };
         mutable Component rotation_{};
         mutable Component rotation_cosine_{};
         mutable Component rotation_sine_{};

         mutable bool is_transformation_dirty_{};
         mutable bool are_trigonometric_values_dirty_{ true };
         mutable bool is_rotation_dirty_{};
         mutable bool is_scale_dirty_{};
   };
}

#endif