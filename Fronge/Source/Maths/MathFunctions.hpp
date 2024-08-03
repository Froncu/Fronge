#if not defined MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Structs/Matrix3x3.hpp"
#include "Maths/Structs/Vector2.hpp"
#include "Maths/Structs/Vector3.hpp"

namespace fro
{
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
	}
}

#endif