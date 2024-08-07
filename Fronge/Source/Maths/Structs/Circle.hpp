#if not defined CIRCLE_HPP
#define CIRCLE_HPP

#include "Maths/Arithmetic.hpp"
#include "Vector2.hpp"

namespace fro
{
	template<Arithmetic Type>
	struct Circle final
	{
	public:
		Vector2<Type> center;
		Type radius;
	};
}

#endif