#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "circle.hpp"
#include "polygon/polygon.hpp"

namespace fro
{
   using Shape = std::variant<
      Circle,
      Polygon>;
}

#endif