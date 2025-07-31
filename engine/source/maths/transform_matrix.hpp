#ifndef TRANSFORM_MATRIX_HPP
#define TRANSFORM_MATRIX_HPP

#include "matrix.hpp"
#include "vector2.hpp"

namespace fro
{
   class TransformMatrix final
   {
      public:
         TransformMatrix() = default;
         FRO_API explicit TransformMatrix(Matrix<double> const& transformation);
         TransformMatrix(TransformMatrix const&) = default;
         TransformMatrix(TransformMatrix&&) noexcept = default;

         ~TransformMatrix() = default;

         TransformMatrix& operator=(TransformMatrix const&) = default;
         TransformMatrix& operator=(TransformMatrix&&) noexcept = default;
         FRO_API [[nodiscard]] TransformMatrix operator*(TransformMatrix const& other) const;
         FRO_API TransformMatrix& operator*=(TransformMatrix const& other);
         FRO_API [[nodiscard]] bool operator==(TransformMatrix const& other) const;

         FRO_API void translate(Vector2<double> translation);
         FRO_API void rotate(double rotation_radians);
         FRO_API void scale(Vector2<double> scale);

         FRO_API [[nodiscard]] TransformMatrix translated(Vector2<double> translation) const;
         FRO_API [[nodiscard]] TransformMatrix rotated(double rotation_radians) const;
         FRO_API [[nodiscard]] TransformMatrix scaled(Vector2<double> scale) const;

         FRO_API void change_translation(Vector2<double> position);
         FRO_API void change_rotation(double rotation_radians);
         FRO_API void change_scale(Vector2<double> scale);

         FRO_API [[nodiscard]] Matrix<double> const& transformation() const;
         FRO_API [[nodiscard]] Vector2<double> translation() const;
         FRO_API [[nodiscard]] double rotation() const;
         FRO_API [[nodiscard]] Vector2<double> scale() const;

         FRO_API [[nodiscard]] TransformMatrix inversed() const;
         FRO_API TransformMatrix& inverse();

      private:
         void calculate_rotation() const;
         void calculate_scale() const;

         mutable Matrix<double> transformation_{ true };

         mutable Vector2<double> scale_{ 1.0, 1.0 };
         mutable double rotation_{};
         mutable double rotation_cosine_{};
         mutable double rotation_sine_{};

         mutable bool is_transformation_dirty_{};
         mutable bool are_trigonometric_values_dirty_{ true };
         mutable bool is_rotation_dirty_{};
         mutable bool is_scale_dirty_{};
   };
}

#endif