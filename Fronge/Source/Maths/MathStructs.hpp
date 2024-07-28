#if not defined MATH_STRUCTS_HPP
#define MATH_STRUCTS_HPP

#include "froch.hpp"

namespace fro
{
	template<typename Type>
	concept Arithmetic =
		std::is_arithmetic_v<Type>;

	template<Arithmetic Type>
	struct Vector2 final
	{
		Type& operator[](std::size_t const index)
		{
			switch (index)
			{
			case 0:
				return x;

			case 1:
				return y;

			default:
				FRO_EXCEPTION("index {} outside [0, 1] range!", index);
			}
		}

		Type operator[](std::size_t const index) const
		{
			switch (index)
			{
			case 0:
				return x;

			case 1:
				return y;

			default:
				FRO_EXCEPTION("index {} outside [0, 1] range!", index);
			}
		}

		template<Arithmetic OtherType>
		auto operator*(Vector2<OtherType> const& vector) const
		{
			return
				x * vector.x +
				y * vector.y;
		}

		Type x;
		Type y;
	};

	template<Arithmetic Type>
	struct Vector3 final
	{
		Type& operator[](std::size_t const index)
		{
			switch (index)
			{
			case 0:
				return x;

			case 1:
				return y;

			case 2:
				return z;

			default:
				FRO_EXCEPTION("index {} outside [0, 2] range!", index);
			}
		}

		Type operator[](std::size_t const index) const
		{
			switch (index)
			{
			case 0:
				return x;

			case 1:
				return y;

			case 2:
				return z;

			default:
				FRO_EXCEPTION("index {} outside [0, 2] range!", index);
			}
		}

		template<Arithmetic OtherType>
		auto operator*(Vector3<OtherType> const& vector) const
		{
			return
				x * vector.x +
				y * vector.y +
				z * vector.z;
		}

		Type x;
		Type y;
		Type z;
	};

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
		FRO_NODISCARD auto operator*(Matrix3x3<OtherType> const& matrix) const
		{
			using ResultType = decltype(std::declval<Type>()* std::declval<OtherType>());

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
			return *this = *this * matrix;
		}

	private:
		RowType mData[3];
	};

	template<Arithmetic Type>
	struct Rectangle final
	{
		Type x;
		Type y;
		Type width;
		Type height;
	};
}

#endif