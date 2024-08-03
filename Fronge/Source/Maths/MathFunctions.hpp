#if not defined MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Structs/Polygon.hpp"

namespace fro
{
	namespace math
	{
		template<Arithmetic Type>
		FRO_NODISCARD constexpr auto degrees(Type const& radians)
		{
			double const degrees{ radians / std::numbers::pi * 180 };

			if constexpr (std::same_as<Type, float>)
				return static_cast<float>(degrees);

			return degrees;
		}

		template<Arithmetic Type>
		FRO_NODISCARD constexpr auto radians(Type const& degrees)
		{
			double const radians{ degrees / 180.0 * std::numbers::pi };

			if constexpr (std::same_as<Type, float>)
				return static_cast<float>(radians);
			else
				return radians;
		}

		template<Arithmetic Type1, Arithmetic Type2>
		FRO_NODISCARD Vector2<Type1> getFurthestVertexInDirection(Polygon<Type1> const& polygon, Vector2<Type2> const& direction)
		{
			auto maxDot{ std::numeric_limits<decltype(std::declval<Vector2<Type1>>() * std::declval<Vector2<Type2>>())>().lowest() };

			Vector2<Type1> furthestVertex;
			for (Vector2<Type1> const& vertex : polygon.vertices)
			{
				auto const dot{ vertex * direction };

				if (dot <= maxDot)
					continue;

				maxDot = dot;
				furthestVertex = vertex;
			}

			return furthestVertex;
		}

		template<Arithmetic Type1, Arithmetic Type2, Arithmetic Type3>
		FRO_NODISCARD auto getTripleProduct(Vector2<Type1> const& vector1, Vector2<Type2> const& vector2, Vector2<Type3> const& vector3)
		{
			Vector3<Type1> const vector13D{ vector1.x, vector1.y, static_cast<Type1>(0) };
			Vector3<Type2> const vector23D{ vector2.x, vector2.y, static_cast<Type1>(0) };
			Vector3<Type2> const vector33D{ vector3.x, vector3.y, static_cast<Type1>(0) };

			Vector3 const result{ vector13D.getCross(vector23D).getCross(vector33D) };
			return Vector2{ result.x, result.y };
		}

		template<Arithmetic Type1, Arithmetic Type2>
		FRO_NODISCARD bool GJK(Polygon<Type1> const& polygon1, Polygon<Type2> const& polygon2)
		{
			Vector2<double> direction{ 1.0, 0.0 };

			auto point{ getFurthestVertexInDirection(polygon1, direction) - getFurthestVertexInDirection(polygon2, -direction) };
			std::vector simplex{ point };

			direction = -simplex.back();
			while (true)
			{
				point = getFurthestVertexInDirection(polygon1, direction) - getFurthestVertexInDirection(polygon2, -direction);

				if (point * direction < 0)
					return false;

				simplex.push_back(point);

				auto const A{ simplex.back() };
				auto const AO{ -A };

				if (simplex.size() == 3)
				{
					auto const B{ simplex[1] };
					auto const C{ simplex[0] };

					auto const AB{ B - A };
					auto const AC{ C - A };

					auto const ABPerp{ getTripleProduct(AC, AB, AB) };
					auto const ACPerp{ getTripleProduct(AB, AC, AC) };

					if (ABPerp * AO > 0)
					{
						simplex.erase(simplex.begin());
						direction = ABPerp;
					}
					else if (ACPerp * AO > 0)
					{
						simplex.erase(simplex.begin() + 1);
						direction = ACPerp;
					}
					else
						return true;
				}
				else
				{
					Vector2<Type1> const B{ simplex[0] };
					Vector2<Type1> const AB{ B - A };
					direction = getTripleProduct(AB, AO, AB);
				}
			}
		}
	}
}

#endif