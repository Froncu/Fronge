#include "froch.hpp"
#include "transform_matrix.hpp"

namespace fro
{
   TransformMatrix::TransformMatrix(Matrix<double> const& transformation)
      : transformation_{ transformation }
      , is_rotation_dirty_{ true }
      , is_scale_dirty_{ true }
   {
   }

   TransformMatrix TransformMatrix::operator*(TransformMatrix const& other) const
   {
      return transformation() * other.transformation();
   }

   TransformMatrix& TransformMatrix::operator*=(TransformMatrix const& other)
   {
      return *this = other * *this;
   }

   bool TransformMatrix::operator==(TransformMatrix const& other) const
   {
      return transformation() == other.transformation();
   }

   TransformMatrix::operator Matrix<double> const&() const
   {
      return transformation();
   }

   void TransformMatrix::translate(Vector2<double> const translation)
   {
      transformation_[0][2] += translation.x;
      transformation_[1][2] += translation.y;
   }

   void TransformMatrix::rotate(double const rotation_radians)
   {
      change_rotation(rotation() + rotation_radians);
   }

   void TransformMatrix::scale(Vector2<double> const scale)
   {
      auto const [x, y]{ this->scale() };
      change_scale({ x * scale.x, y * scale.y });
   }

   void TransformMatrix::change_translation(Vector2<double> const position)
   {
      transformation_[0][2] = position.x;
      transformation_[1][2] = position.y;
   }

   void TransformMatrix::change_rotation(double const rotation_radians)
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
   }

   void TransformMatrix::change_scale(Vector2<double> const scale)
   {
      if (is_rotation_dirty_)
      {
         calculate_rotation();
         is_rotation_dirty_ = false;
      }

      scale_ = scale;
      is_scale_dirty_ = false;

      is_transformation_dirty_ = true;
   }

   Vector2<double> TransformMatrix::translation() const
   {
      return { transformation_[0][2], transformation_[1][2] };
   }

   double TransformMatrix::rotation() const
   {
      if (is_rotation_dirty_)
      {
         calculate_rotation();
         is_rotation_dirty_ = false;
      }

      return rotation_;
   }

   Vector2<double> TransformMatrix::scale() const
   {
      if (is_scale_dirty_)
      {
         calculate_scale();
         is_scale_dirty_ = false;
      }

      return scale_;
   }

   TransformMatrix TransformMatrix::inversed() const
   {
      return transformation().inversed();
   }

   TransformMatrix& TransformMatrix::inverse()
   {
      return *this = inversed();
   }

   Matrix<double> const& TransformMatrix::transformation() const
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
      Matrix<double> const& transformation{ this->transformation() };
      rotation_ = std::atan2(transformation[1][0], transformation[0][0]);
   }

   void TransformMatrix::calculate_scale() const
   {
      Matrix<double> const& transformation{ this->transformation() };
      scale_.x = Vector2{ transformation[0][0], transformation[1][0] }.magnitude();
      scale_.y = Vector2{ transformation[0][1], transformation[1][1] }.magnitude();
   }
}