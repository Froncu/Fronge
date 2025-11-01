#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include "fronge/pch.hpp"

namespace fro
{
   template <typename T>
   concept Arithmetic = std::is_arithmetic_v<T>;
}

#endif