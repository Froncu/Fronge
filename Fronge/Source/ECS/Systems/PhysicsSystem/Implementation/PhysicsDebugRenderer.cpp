#include "froch.hpp"

#include "PhysicsDebugRenderer.hpp"

namespace fro
{
	void PhysicsDebugRenderer::DrawPolygon(b2Vec2 const* const vertices, int32 const vertexCount, b2Color const& color)
	{
		Polygon<double> polygon{};
		polygon.vertices.resize(vertexCount);

		for (std::size_t index{}; index < vertexCount; ++index)
		{
			polygon.vertices[index].x = static_cast<double>(vertices[index].x);
			polygon.vertices[index].y = static_cast<double>(vertices[index].y);
		}

		mRenderer->renderPolygon(polygon, false, color.r, color.g, color.b, color.a);
	}

	void PhysicsDebugRenderer::DrawSolidPolygon(b2Vec2 const* const vertices, int32 const vertexCount, b2Color const& color)
	{
		Polygon<double> polygon{};
		polygon.vertices.resize(vertexCount);

		for (std::size_t index{}; index < vertexCount; ++index)
		{
			polygon.vertices[index].x = static_cast<double>(vertices[index].x);
			polygon.vertices[index].y = static_cast<double>(vertices[index].y);
		}

		mRenderer->renderPolygon(polygon, true, color.r, color.g, color.b, color.a);
	}

	void PhysicsDebugRenderer::DrawCircle(b2Vec2 const& center, float const radius, b2Color const& color)
	{
		Circle<double> const circle
		{
			.center{ center.x, center.y },
			.radius{ radius }
		};

		mRenderer->renderCircle(circle, false, color.r, color.g, color.b, color.a);
	}

	void PhysicsDebugRenderer::DrawSolidCircle(b2Vec2 const& center, float const radius, b2Vec2 const& axis, b2Color const& color)
	{
		Circle<double> const circle
		{
			.center{ center.x, center.y },
			.radius{ radius }
		};

		mRenderer->renderCircle(circle, true, color.r, color.g, color.b, color.a);

		auto const secondVertex{ center + radius * axis };
		Chain<double> const convertedAxis{ .vertices{ { center.x, center.y }, { secondVertex.x, secondVertex.y } } };
		mRenderer->renderChain(convertedAxis, 2.0, color.r * 0.8, color.g * 0.8, color.b * 0.8, color.a);
	}

	void PhysicsDebugRenderer::DrawSegment(b2Vec2 const& p1, b2Vec2 const& p2, b2Color const& color)
	{
		Chain<double> const segment
		{
			.vertices
			{
				{ p1.x, p1.y },
				{ p2.x, p2.y }
			}
		};

		mRenderer->renderChain(segment, 2.0,
			color.r,
			color.g,
			color.b,
			color.a);
	}

	void PhysicsDebugRenderer::DrawTransform(b2Transform const&)
	{
	}

	void PhysicsDebugRenderer::DrawPoint(b2Vec2 const& p, float const size, b2Color const& color)
	{
		Circle<double> circle
		{
			.center{ p.x, p.y },
			.radius{ size }
		};

		mRenderer->renderCircle(circle, true, color.r, color.g, color.b, color.a);
	}
}