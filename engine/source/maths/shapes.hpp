#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "froch.hpp"
#include "maths/vector2.hpp"

namespace fro
{
   struct Circle final
   {
      double radius;
   };

   struct Polygon final
   {
      std::vector<Vector2<double>> vertices;
   };

   using Shape = std::variant<Circle, Polygon>;
}

#endif