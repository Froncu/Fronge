#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Structs/Vector2.hpp"
#include "Maths/Structs/Vector3.hpp"

namespace fro
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

	template<Arithmetic Type1, Arithmetic Type2, Arithmetic Type3, Arithmetic Type4>
	FRO_NODISCARD static bool doLinesIntersect(Vector2<Type1> const p1,
		Vector2<Type2> const p2,
		Vector2<Type3> const q1,
		Vector2<Type4> const q2)
	{
		// NOTE: I completely forgot how line intersection detection works,
		// so I have no idea what's happening here no time left to dig into it,
		// thanks ChatGPT!

		Vector2 const p2p1{ p1 - p2 };
		Vector2 const q2q1{ q1 - q2 };
		Vector2 const p1q1{ q1 - p1 };

		auto const cross1{ p2p1.getCross(q2q1) };
		auto const cross2{ p2p1.getCross(p1q1) };
		auto const cross3{ q2q1.getCross(p1q1) };

		double constexpr epsilon{ std::numeric_limits<decltype(cross1)>::epsilon() };

		if (std::abs(cross1) < epsilon)
		{
			auto const isBetween
			{
				[](auto const a, auto const b, auto const c)
				{
					return std::min(a, b) <= c and c <= std::max(a, b);
				}
			};

			return
				(isBetween(p1.x, p2.x, q1.x) and isBetween(p1.y, p2.y, q1.y)) or
				(isBetween(p1.x, p2.x, q2.x) and isBetween(p1.y, p2.y, q2.y)) or
				(isBetween(q1.x, q2.x, p1.x) and isBetween(q1.y, q2.y, p1.y)) or
				(isBetween(q1.x, q2.x, p2.x) and isBetween(q1.y, q2.y, p2.y));
		}
		else
		{
			double const inverseCross1{ 1.0 / cross1 };
			double const t{ cross3 * inverseCross1 };
			double const u{ cross2 * inverseCross1 };

			return
				t >= 0.0 and
				t <= 1.0 and
				u >= 0.0 and
				u <= 1.0;
		}
	}

	template<Arithmetic Type1, Arithmetic Type2, Arithmetic Type3>
	FRO_NODISCARD static bool doesIntersectPolygon(Vector2<Type1> const p1,
		Vector2<Type2> const p2,
		Polygon<Type3> const& polygon)
	{
		std::size_t const vertexCount{ polygon.vertices.size() };

		for (std::size_t index{}; index < vertexCount; ++index)
		{
			if (doLinesIntersect(p1,
				p2,
				polygon.vertices[index],
				polygon.vertices[(index + 1) % vertexCount]))
				return true;
		}

		return false;
	}

	template<Arithmetic Type1, Arithmetic Type2, Arithmetic Type3>
	FRO_NODISCARD static bool doesIntersectAnyPolygon(Vector2<Type1> const p1,
		Vector2<Type2> const p2,
		std::vector<fro::Polygon<Type3>> const& polygons)
	{
		for (Polygon<double> const& polygon : polygons)
			if (doesIntersectPolygon(p1, p2, polygon))
				return true;

		return false;
	}
}

#endif