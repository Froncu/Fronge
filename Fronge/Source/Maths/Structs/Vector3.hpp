#if not defined VECTOR3_HPP
#define VECTOR3_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Arithmetic.hpp"

namespace fro
{
	template<Arithmetic Type>
	struct Vector3 final
	{
		FRO_NODISCARD Type& operator[](std::size_t const index)
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

		FRO_NODISCARD Type operator[](std::size_t const index) const
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
		FRO_NODISCARD constexpr auto operator+(Vector3<OtherType> const& vector) const
		{
			return Vector3
			{
				x + vector.x,
				y + vector.y,
				z + vector.z
			};
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator+=(Vector3<OtherType> const& vector) const
		{
			return *this = *this + vector;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator-(Vector3<OtherType> const& vector) const
		{
			return Vector3
			{
				x - vector.x,
				y - vector.y,
				z - vector.z
			};
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator-=(Vector3<OtherType> const& vector) const
		{
			return *this = *this - vector;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr auto operator*(Vector3<OtherType> const& vector) const
		{
			return
				x * vector.x +
				y * vector.y +
				z * vector.z;
		}

		template<Arithmetic MultiplierType>
		FRO_NODISCARD constexpr auto operator*(MultiplierType const multiplier) const
		{
			return Vector3{ x * multiplier, y * multiplier, z * multiplier };
		}

		template<Arithmetic MultiplierType>
		FRO_NODISCARD constexpr auto operator*=(MultiplierType const multiplier) const
		{
			return *this = *this * multiplier;
		}

		template<Arithmetic DividerType>
		FRO_NODISCARD constexpr auto operator/(DividerType const divider) const
		{
			return Vector3{ x / divider, y / divider, z / divider };
		}

		template<Arithmetic DividerType>
		FRO_NODISCARD constexpr auto operator/=(DividerType const divider) const
		{
			return *this = *this / divider;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD constexpr bool operator==(Vector3<OtherType> const& vector) const
		{
			return
				x == vector.x and
				y == vector.y and
				z == vector.z;
		}

		FRO_NODISCARD auto getMagnitude() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		Type x;
		Type y;
		Type z;
	};
}

#endif