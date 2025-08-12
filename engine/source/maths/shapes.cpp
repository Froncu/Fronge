#include "shapes.hpp"

namespace fro
{
   std::pair<double, double> Polygon::project(Vector2<double> const& normalized_axis) const
   {
      double min{ std::numeric_limits<double>::max() };
      double max{ std::numeric_limits<double>::lowest() };

      for (Vector2<double> const& vertex : vertices)
      {
         double const projection{ vertex * normalized_axis };
         min = std::min(projection, min);
         max = std::max(projection, max);
      }

      return { min, max };
   }

   Vector2<double> Polygon::center() const
   {
      Vector2<double> sum{};
      for (Vector2<double> const& vertex : vertices)
         sum += vertex;

      return sum / vertices.size();
   }
}