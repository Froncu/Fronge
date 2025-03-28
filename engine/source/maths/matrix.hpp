#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "froch.hpp"
#include "vector3.hpp"
#include "arithmetic.hpp"

namespace fro
{
   template <Arithmetic Component>
   class Matrix final
   {
      using Row = Vector3<Component>;

      public:
         Matrix() = default;

         Matrix(Row const& row0, Row const& row1, Row const& row2)
            : data_{ row0, row1, row2 }
         {
         }

         Matrix(Component const a00, Component const a01, Component const a02,
            Component const a10, Component const a11, Component const a12,
            Component const a20, Component const a21, Component const a22)
            : Matrix({ a00, a01, a02 }, { a10, a11, a12 }, { a20, a21, a22 })
         {
         }

         explicit Matrix(bool const identity)
            : Matrix(identity ? static_cast<Component>(1) : 0, 0, 0,
               0, identity ? static_cast<Component>(1) : 0, 0,
               0, 0, identity ? static_cast<Component>(1) : 0)
         {
         }

         Matrix(Matrix const&) = default;
         Matrix(Matrix&&) noexcept = default;

         ~Matrix() = default;

         Matrix& operator=(Matrix const&) = default;
         Matrix& operator=(Matrix&&) noexcept = default;

         template <Arithmetic OtherComponent>
         [[nodiscard]] explicit operator Matrix<OtherComponent>() const
         {
            return
            {
               static_cast<Vector3<OtherComponent>>(data_[0]),
               static_cast<Vector3<OtherComponent>>(data_[1]),
               static_cast<Vector3<OtherComponent>>(data_[2])
            };
         }

         [[nodiscard]] Row& operator[](std::size_t const index)
         {
            if (index > 2)
               exception("index {} is outside the [0, 2] range!", index);

            return data_[index];
         }

         Row const& operator[](std::size_t const index) const
         {
            if (index > 2)
               exception("index {} is outside the [0, 2] range!", index);

            return data_[index];
         }

         template <Arithmetic OtherComponent>
         [[nodiscard]] Vector3<ResultingComponent<Component, OtherComponent>> operator*(
            Vector3<OtherComponent> const& vector) const
         {
            return
            {
               data_[0] * vector,
               data_[1] * vector,
               data_[2] * vector
            };
         }

         template <Arithmetic OtherComponent>
         [[nodiscard]] auto operator*(Matrix<OtherComponent> const& matrix) const
         {
            Matrix<ResultingComponent<Component, OtherComponent>> result{};

            for (std::size_t row_index{}; row_index < 3; ++row_index)
               for (std::size_t column_index{}; column_index < 3; ++column_index)
                  for (std::size_t index{}; index < 3; ++index)
                     result[row_index][column_index] += data_[row_index][index] * matrix[index][column_index];

            return result;
         }

         template <Arithmetic OtherComponent>
            requires std::same_as<ResultingComponent<Component, OtherComponent>, Component>
         Matrix& operator*=(Matrix<OtherComponent> const& matrix)
         {
            return *this = matrix * *this;
         }

         template <Arithmetic OtherComponent>
         [[nodiscard]] bool operator==(Matrix<OtherComponent> const& matrix) const
         {
            for (std::size_t row_index{}; row_index < 3; ++row_index)
               for (std::size_t column_index{}; column_index < 3; ++column_index)
                  if (data_[row_index][column_index] not_eq matrix[row_index][column_index])
                     return false;

            return true;
         }

         [[nodiscard]] Matrix transposed() const
         {
            Matrix result;

            for (std::size_t row_index{}; row_index < 3; ++row_index)
               for (std::size_t column_index{}; column_index < 3; ++column_index)
                  result[row_index][column_index] = data_[column_index][row_index];

            return result;
         }

         Matrix& transpose()
         {
            return *this = transposed();
         }

         [[nodiscard]] auto determinant() const
         {
            return
               data_[0][0] * (data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1]) -
               data_[0][1] * (data_[1][0] * data_[2][2] - data_[1][2] * data_[2][0]) +
               data_[0][2] * (data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0]);
         }

         [[nodiscard]] auto inversed() const
         {
            using ResultingMatrix = Matrix<ResultingComponent<Component, decltype(1.0f / determinant())>>;

            auto const determinant{ this->determinant() };
            if (not determinant)
               return ResultingMatrix{ true };

            auto const inverse_determinant{ 1.0f / determinant };
            ResultingMatrix result;

            result[0][0] = inverse_determinant * (data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1]);
            result[0][1] = inverse_determinant * (data_[0][2] * data_[2][1] - data_[0][1] * data_[2][2]);
            result[0][2] = inverse_determinant * (data_[0][1] * data_[1][2] - data_[0][2] * data_[1][1]);

            result[1][0] = inverse_determinant * (data_[1][2] * data_[2][0] - data_[1][0] * data_[2][2]);
            result[1][1] = inverse_determinant * (data_[0][0] * data_[2][2] - data_[0][2] * data_[2][0]);
            result[1][2] = inverse_determinant * (data_[0][2] * data_[1][0] - data_[0][0] * data_[1][2]);

            result[2][0] = inverse_determinant * (data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0]);
            result[2][1] = inverse_determinant * (data_[0][1] * data_[2][0] - data_[0][0] * data_[2][1]);
            result[2][2] = inverse_determinant * (data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0]);

            return result;
         }

         Matrix& inverse()
            requires std::same_as<ResultingComponent<Component, decltype(1.0f / determinant())>, Component>
         {
            return *this = inversed();
         }

      private:
         std::array<Row, 3> data_;
   };
}

#endif