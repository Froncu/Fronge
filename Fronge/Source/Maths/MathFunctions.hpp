#if not defined MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Structs/Matrix3x3.hpp"
#include "Maths/Structs/Vector2.hpp"
#include "Maths/Structs/Vector3.hpp"

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

	template<Arithmetic Type, Arithmetic MultiplierType>
	FRO_NODISCARD auto operator*(Vector2<Type> const vector, MultiplierType const multiplier)
	{
		return Vector2{ vector.x * multiplier, vector.y * multiplier };
	}

	template<Arithmetic Type, Arithmetic MultiplierType>
	FRO_NODISCARD auto operator*(Vector3<Type> const vector, MultiplierType const multiplier)
	{
		return Vector3{ vector.x * multiplier, vector.y * multiplier, vector.z * multiplier };
	}

	template<Arithmetic Type, Arithmetic DividerType>
	FRO_NODISCARD auto operator/(Vector2<Type> const vector, DividerType const divider)
	{
		return Vector2{ vector.x / divider, vector.y / divider };
	}

	template<Arithmetic Type, Arithmetic DividerType>
	FRO_NODISCARD auto operator/(Vector3<Type> const vector, DividerType const divider)
	{
		return Vector3{ vector.x / divider, vector.y / divider, vector.z / divider };
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

			decltype(cosine) constexpr zero{};
			return Matrix3x3
			{
				cosine, -sine, zero,
				sine, cosine, zero,
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
		FRO_NODISCARD constexpr Matrix3x3<Type>& transpose(Matrix3x3<Type>& matrix)
		{
			Matrix3x3<Type> result;

			for (std::size_t rowIndex{}; rowIndex < 3; ++rowIndex)
				for (std::size_t columnIndex{}; columnIndex < 3; ++columnIndex)
					result[rowIndex][columnIndex] = matrix[columnIndex][rowIndex];

			matrix = result;
			return matrix;
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr auto getMagnitude(Vector2<Type> const& vector)
		{
			return std::sqrt(vector.x * vector.x + vector.y * vector.y);
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr auto getMagnitude(Vector3<Type> const& vector)
		{
			return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		}
	}
}

#endif