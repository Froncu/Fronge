#if not defined MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Vector3.hpp"
#include "Maths/Arithmetic.hpp"

namespace fro
{
	template<Arithmetic Type>
	class Matrix3x3 final
	{
		using RowType = Vector3<Type>;

	public:
		constexpr Matrix3x3() = default;
		constexpr Matrix3x3(RowType const& row0, RowType const& row1, RowType const& row2)
			: mData{ row0, row1, row2 }
		{
		}

		constexpr Matrix3x3(Type const a00, Type const a01, Type const a02,
			Type const a10, Type const a11, Type const a12,
			Type const a20, Type const a21, Type const a22)
			: Matrix3x3({ a00, a01, a02 }, { a10, a11, a12 }, { a20, a21, a22 })
		{
		}

		constexpr Matrix3x3(bool const identity)
			: Matrix3x3(identity ? static_cast<Type>(1) : 0, 0, 0,
				0, identity ? static_cast<Type>(1) : 0, 0,
				0, 0, identity ? static_cast<Type>(1) : 0)
		{
		}

		constexpr Matrix3x3(Matrix3x3 const&) = default;
		constexpr Matrix3x3(Matrix3x3&&) noexcept = default;

		~Matrix3x3() = default;

		constexpr Matrix3x3& operator=(Matrix3x3 const&) = default;
		constexpr Matrix3x3& operator=(Matrix3x3&&) noexcept = default;
		constexpr RowType& operator[](std::size_t const index)
		{
			return mData[index];
		}

		constexpr RowType const& operator[](std::size_t const index) const
		{
			return mData[index];
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD bool operator==(Matrix3x3<OtherType> const& matrix) const
		{
			for (std::size_t rowIndex{}; rowIndex < 3; ++rowIndex)
				for (std::size_t columnIndex{}; columnIndex < 3; ++columnIndex)
					if (mData[rowIndex][columnIndex] not_eq matrix[rowIndex][columnIndex])
						return false;

			return true;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD auto operator*(Matrix3x3<OtherType> const& matrix) const
		{
			using ResultType = decltype(std::declval<Type>() * std::declval<OtherType>());

			Matrix3x3<ResultType> result{};

			for (std::size_t rowIndex{}; rowIndex < 3; ++rowIndex)
				for (std::size_t columnIndex{}; columnIndex < 3; ++columnIndex)
					for (std::size_t index{}; index < 3; ++index)
						result[rowIndex][columnIndex] += mData[rowIndex][index] * matrix[index][columnIndex];

			return result;
		}

		template<Arithmetic OtherType>
		auto operator*=(Matrix3x3<OtherType> const& matrix)
		{
			return *this = matrix * *this;
		}

		FRO_NODISCARD Matrix3x3<Type> getTransposed() const
		{
			Matrix3x3<Type> result;

			for (std::size_t rowIndex{}; rowIndex < 3; ++rowIndex)
				for (std::size_t columnIndex{}; columnIndex < 3; ++columnIndex)
					result[rowIndex][columnIndex] = mData[columnIndex][rowIndex];

			return result;
		}

		Matrix3x3<Type>& transpose()
		{
			return *this = getTransposed();
		}

		FRO_NODISCARD Matrix3x3<double> getInverse() const
		{
			Type const determinant{ getDeterminant() };

			if (determinant == 0)
				return true;

			double const inverseDeterminant{ 1.0 / determinant };
			Matrix3x3<double> inverse;

			inverse[0][0] = inverseDeterminant * (mData[1][1] * mData[2][2] - mData[1][2] * mData[2][1]);
			inverse[0][1] = inverseDeterminant * (mData[0][2] * mData[2][1] - mData[0][1] * mData[2][2]);
			inverse[0][2] = inverseDeterminant * (mData[0][1] * mData[1][2] - mData[0][2] * mData[1][1]);

			inverse[1][0] = inverseDeterminant * (mData[1][2] * mData[2][0] - mData[1][0] * mData[2][2]);
			inverse[1][1] = inverseDeterminant * (mData[0][0] * mData[2][2] - mData[0][2] * mData[2][0]);
			inverse[1][2] = inverseDeterminant * (mData[0][2] * mData[1][0] - mData[0][0] * mData[1][2]);

			inverse[2][0] = inverseDeterminant * (mData[1][0] * mData[2][1] - mData[1][1] * mData[2][0]);
			inverse[2][1] = inverseDeterminant * (mData[0][1] * mData[2][0] - mData[0][0] * mData[2][1]);
			inverse[2][2] = inverseDeterminant * (mData[0][0] * mData[1][1] - mData[0][1] * mData[1][0]);

			return inverse;
		}

		Matrix3x3<double>& inverse()
		{
			if (Matrix3x3<double> inverse{ getInverse() }; inverse == Matrix3x3<double>{ true })
				*this = std::move(*inverse);

			return *this;
		}

		FRO_NODISCARD Type getDeterminant() const
		{
			return
				mData[0][0] * (mData[1][1] * mData[2][2] - mData[1][2] * mData[2][1]) -
				mData[0][1] * (mData[1][0] * mData[2][2] - mData[1][2] * mData[2][0]) +
				mData[0][2] * (mData[1][0] * mData[2][1] - mData[1][1] * mData[2][0]);
		}

	private:
		RowType mData[3];
	};
}

#endif