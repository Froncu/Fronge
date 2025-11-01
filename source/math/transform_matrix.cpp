#include "fronge/pch.hpp"
#include "fronge/transform_matrix.hpp"

namespace fro
{
   TransformMatrix::TransformMatrix(Matrix const& transformation)
      : transformation_{ transformation }
      , is_rotation_dirty_{ true }
      , is_scale_dirty_{ true }
   {
   }

   Matrix<double>::Row TransformMatrix::operator[](std::size_t const index) const
   {
      return transformation()[index];
   }

   Vector3<TransformMatrix::Component> TransformMatrix::operator*(Vector3<Component> const& vector) const
   {
      return transformation() * vector;
   }

   TransformMatrix TransformMatrix::operator*(TransformMatrix const& other) const
   {
      return static_cast<TransformMatrix>(transformation() * other.transformation());
   }

   TransformMatrix& TransformMatrix::operator*=(TransformMatrix const& other)
   {
      return *this = other * *this;
   }

   bool TransformMatrix::operator==(TransformMatrix const& other) const
   {
      return transformation() == other.transformation();
   }

   TransformMatrix& TransformMatrix::change_translation(Vector2<Component> const translation)
   {
      transformation_[0][2] = translation.x;
      transformation_[1][2] = translation.y;

      return *this;
   }

   TransformMatrix& TransformMatrix::change_rotation(Component const rotation_radians)
   {
      if (is_scale_dirty_)
      {
         calculate_scale();
         is_scale_dirty_ = false;
      }

      rotation_ = rotation_radians;
      is_rotation_dirty_ = false;

      are_trigonometric_values_dirty_ = true;
      is_transformation_dirty_ = true;

      return *this;
   }

   TransformMatrix& TransformMatrix::change_scale(Vector2<Component> const scale)
   {
      if (is_rotation_dirty_)
      {
         calculate_rotation();
         is_rotation_dirty_ = false;
      }

      scale_ = scale;
      is_scale_dirty_ = false;

      is_transformation_dirty_ = true;

      return *this;
   }

   TransformMatrix TransformMatrix::changed_translation(Vector2<Component> const translation) const
   {
      TransformMatrix transformation{ *this };
      return transformation.change_translation(translation);
   }

   TransformMatrix TransformMatrix::changed_rotation(Component const rotation_radians) const
   {
      TransformMatrix transformation{ *this };
      return transformation.change_rotation(rotation_radians);
   }

   TransformMatrix TransformMatrix::changed_scale(Vector2<Component> const scale) const
   {
      TransformMatrix transformation{ *this };
      return transformation.change_scale(scale);
   }

   TransformMatrix& TransformMatrix::translate(Vector2<Component> const translation)
   {
      return change_translation(this->translation() + translation);
   }

   TransformMatrix& TransformMatrix::rotate(Component const rotation_radians)
   {
      return change_rotation(rotation() + rotation_radians);
   }

   TransformMatrix& TransformMatrix::scale(Vector2<Component> const scale)
   {
      auto const [x, y]{ this->scale() };
      return change_scale({ x * scale.x, y * scale.y });
   }

   TransformMatrix TransformMatrix::translated(Vector2<Component> const translation) const
   {
      TransformMatrix transformation{ *this };
      return transformation.translate(translation);
   }

   TransformMatrix TransformMatrix::rotated(Component const rotation_radians) const
   {
      TransformMatrix transformation{ *this };
      return transformation.rotate(rotation_radians);
   }

   TransformMatrix TransformMatrix::scaled(Vector2<Component> const scale) const
   {
      TransformMatrix transformation{ *this };
      return transformation.scale(scale);
   }

   Vector2<TransformMatrix::Component> TransformMatrix::translation() const
   {
      return { transformation_[0][2], transformation_[1][2] };
   }

   TransformMatrix::Component TransformMatrix::rotation() const
   {
      if (is_rotation_dirty_)
      {
         calculate_rotation();
         is_rotation_dirty_ = false;
      }

      return rotation_;
   }

   Vector2<TransformMatrix::Component> TransformMatrix::scale() const
   {
      if (is_scale_dirty_)
      {
         calculate_scale();
         is_scale_dirty_ = false;
      }

      return scale_;
   }

   TransformMatrix TransformMatrix::transposed() const
   {
      return static_cast<TransformMatrix>(transformation().transposed());
   }

   TransformMatrix& TransformMatrix::transpose()
   {
      return *this = transposed();
   }

   TransformMatrix TransformMatrix::inversed() const
   {
      return static_cast<TransformMatrix>(transformation().inversed());
   }

   TransformMatrix& TransformMatrix::inverse()
   {
      return *this = inversed();
   }

   TransformMatrix::Matrix const& TransformMatrix::transformation() const
   {
      if (is_transformation_dirty_)
      {
         if (are_trigonometric_values_dirty_)
         {
            rotation_cosine_ = std::cos(rotation_);
            rotation_sine_ = std::sin(rotation_);

            are_trigonometric_values_dirty_ = false;
         }

         transformation_[0][0] = scale_.x * rotation_cosine_;
         transformation_[0][1] = -scale_.y * rotation_sine_;
         transformation_[1][0] = scale_.x * rotation_sine_;
         transformation_[1][1] = scale_.y * rotation_cosine_;

         is_transformation_dirty_ = false;
      }

      return transformation_;
   }

   void TransformMatrix::calculate_rotation() const
   {
      Matrix const& matrix{ transformation() };
      rotation_ = std::atan2(matrix[1][0], matrix[0][0]);
   }

   void TransformMatrix::calculate_scale() const
   {
      Matrix const& matrix{ transformation() };
      scale_.x = Vector2{ matrix[0][0], matrix[1][0] }.magnitude();
      scale_.y = Vector2{ matrix[0][1], matrix[1][1] }.magnitude();
   }
}