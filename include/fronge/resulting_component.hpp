#ifndef RESULTING_COMPONENT_HPP
#define RESULTING_COMPONENT_HPP

#include "fronge/arithmetic.hpp"

namespace fro
{
   template <Arithmetic ComponentA, Arithmetic ComponentB>
   using ResultingComponent = decltype(std::declval<ComponentA>() * std::declval<ComponentB>());
}

#endif