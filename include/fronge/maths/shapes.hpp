#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "fronge/maths/circle.hpp"
#include "fronge/maths/polygon.hpp"

namespace fro
{
   using Shape = std::variant<
      Circle,
      Polygon>;
}

#endif