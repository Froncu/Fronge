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
}

#endif