#ifndef CHAIN_HPP
#define CHAIN_HPP

#include "froch.hpp"

#include "Maths/Arithmetic.hpp"
#include "Maths/Structs/Vector2.hpp"

namespace fro
{
	template<Arithmetic Type>
	class Chain final
	{
	public:
		std::vector<Vector2<Type>> vertices{};
	};
}

#endif