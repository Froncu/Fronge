#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "fronge/api.hpp"
#include "fronge/pch.hpp"
#include "fronge/vector2.hpp"

namespace fro
{
   struct Polygon final
   {
      FRO_API [[nodiscard]] std::pair<double, double> project(Vector2<double> const& normalized_axis) const;
      FRO_API [[nodiscard]] Vector2<double> center() const;

      std::vector<Vector2<double>> vertices{};
   };
}

#endif