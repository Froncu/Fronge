#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include "froch.hpp"

namespace fro
{
	template<typename Type>
	concept Arithmetic =
		std::is_arithmetic_v<Type>;
}

#endif