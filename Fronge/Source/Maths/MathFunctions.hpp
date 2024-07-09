#if not defined MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "MathStructs.hpp"

namespace fro
{
	template<Arithmetic Type1, Arithmetic Type2>
	FRO_NODISCARD auto operator*(Matrix3x3<Type1> const& matrix, Vector3<Type2> const& vector)
	{
		return Vector3
		{
			matrix[0] * vector,
			matrix[1] * vector,
			matrix[2] * vector
		};
	}

	namespace math
	{
		template<Arithmetic Type>
		FRO_NODISCARD constexpr auto degrees(Type const& radians)
		{
			double const degrees{ radians / std::numbers::pi * 180 };

			if constexpr (std::same_as<Type, float>)
				return static_cast<float>(degrees);

			return degrees;
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr auto radians(Type const& degrees)
		{
			double const radians{ degrees / 180.0 * std::numbers::pi };

			if constexpr (std::same_as<Type, float>)
				return static_cast<float>(radians);
			else
				return radians;
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr Matrix3x3<Type> createTranslator(Type const& x, Type const& y)
		{
			return
			{
				static_cast<Type>(1), 0, x,
				0, static_cast<Type>(1), y,
				0, 0, static_cast<Type>(1)
			};
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr Matrix3x3<Type> createTranslator(Vector2<Type> const& translation)
		{
			return createTranslator(translation.x, translation.y);
		}

		template<Arithmetic Type>
		FRO_NODISCARD auto createRotator(Type const& rotation)
		{
			auto const cosine{ std::cos(rotation) };
			auto const sine{ std::sin(rotation) };

			decltype(cosine) const zero{};
			return Matrix3x3
			{
				cosine, sine, zero,
				-sine, cosine, zero,
				zero, zero, static_cast<decltype(cosine)>(1)
			};
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr Matrix3x3<Type> createScalar(Type const& x, Type const& y)
		{
			return
			{
				x, 0, 0,
				0, y, 0,
				0, 0, static_cast<Type>(1)
			};
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr Matrix3x3<Type> createScalar(Vector2<Type> const& scale)
		{
			return createScalar(scale.x, scale.y);
		}

		template<Arithmetic Type>
		constexpr Matrix3x3<Type>& transpose(Matrix3x3<Type>& matrix)
		{
			Matrix3x3<Type> result;

			for (std::size_t rowIndex{}; rowIndex < 3; ++rowIndex)
				for (std::size_t columnIndex{}; columnIndex < 3; ++columnIndex)
					result[rowIndex][columnIndex] = matrix[columnIndex][rowIndex];

			matrix = result;
			return matrix;
		}
	}
}

#endif