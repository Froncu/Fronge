#ifndef DISPATCHABLE_HPP
#define DISPATCHABLE_HPP

#include "fronge/pch.hpp"

namespace fro
{
   template <typename Type>
   concept Dispatchable = not std::is_reference_v<Type>;
}

#endif