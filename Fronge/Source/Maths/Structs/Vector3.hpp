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
		FRO_NODISCARD auto operator*(Vector3<OtherType> const& vector) const
		{
			return
				x * vector.x +
				y * vector.y +
				z * vector.z;
		}

		template<Arithmetic OtherType>
		FRO_NODISCARD bool operator==(Vector3<OtherType> const& vector) const
		{
			return
				x == vector.x and
				y == vector.y and
				z == vector.z;
		}

		Type x;
		Type y;
		Type z;
	};
}

#endif