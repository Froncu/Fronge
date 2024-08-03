#if not defined VECTOR2_HPP
#define VECTOR2_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Arithmetic.hpp"

namespace fro
{
	template<Arithmetic Type>
	struct Vector2 final
	{
		FRO_NODISCARD Type& operator[](std::size_t const index)
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

		FRO_NODISCARD Type operator[](std::size_t const index) const
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
		FRO_NODISCARD constexpr auto operator+(Vector2<OtherType> const& vector) const
		{
			return Vector2
			{
				x + vector.x,
				y + vector.y
			};
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator+=(Vector2<OtherType> const& vector)
		{
			return *this = *this + vector;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator-(Vector2<OtherType> const& vector) const
		{
			return Vector2
			{
				x - vector.x,
				y - vector.y
			};
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator-=(Vector2<OtherType> const& vector)
		{
			return *this = *this = vector;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator*(Vector2<OtherType> const& vector) const
		{
			return
				x * vector.x +
				y * vector.y;
		}

		template<Arithmetic MultiplierType>
		FRO_NODISCARD constexpr auto operator*(MultiplierType const multiplier) const
		{
			return Vector2{ x * multiplier, y * multiplier };
		}

		template<Arithmetic MultiplierType>
		FRO_NODISCARD constexpr auto operator*=(MultiplierType const multiplier) const
		{
			return *this = *this * multiplier;
		}

		template<Arithmetic DividerType>
		FRO_NODISCARD constexpr auto operator/(DividerType const divider) const
		{
			return Vector2{ x / divider, y / divider };
		}

		template<Arithmetic DividerType>
		FRO_NODISCARD constexpr auto operator/=(DividerType const divider) const
		{
			return *this = *this / divider;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr bool operator==(Vector2<OtherType> const& vector) const
		{
			return
				x == vector.x and
				y == vector.y;
		}

		FRO_NODISCARD auto getMagnitude() const
		{
			return std::sqrt(x * x + y * y);
		}

		Type x;
		Type y;
	};
}

#endif