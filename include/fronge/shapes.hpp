#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "fronge/circle.hpp"
#include "fronge/polygon.hpp"

namespace fro
{
   using Shape = std::variant<
      Circle,
      Polygon>;
}

#endif