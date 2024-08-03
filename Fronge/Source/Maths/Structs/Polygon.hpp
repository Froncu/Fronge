#if not defined POLYGON_HPP
#define POLYGON_HPP

#include "froch.hpp"

#include "Maths/Arithmetic.hpp"
#include "Maths/Structs/Vector2.hpp"

namespace fro
{
	template<Arithmetic Type>
	struct Polygon final
	{
	public:
		std::vector<Vector2<Type>> vertices{};
	};
}

#endif