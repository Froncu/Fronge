#if not defined RECTANGLE_HPP
#define RECTANGLE_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Arithmetic.hpp"

namespace fro
{
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