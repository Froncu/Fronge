#if not defined VECTOR3_HPP
#define VECTOR3_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Utility/Concepts.hpp"

namespace fro
{
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
}

#endif