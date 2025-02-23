#ifndef PHYSICS_DEBUG_RENDERER_HPP
#define PHYSICS_DEBUG_RENDERER_HPP

#include "Core.hpp"
#include "Renderer/Renderer.hpp"

#include <box2d/b2_draw.h>

namespace fro
{
	class PhysicsDebugRenderer final : public b2Draw
	{
	public:
		PhysicsDebugRenderer() = default;
		FRO_API PhysicsDebugRenderer(PhysicsDebugRenderer const& other);
		FRO_API PhysicsDebugRenderer(PhysicsDebugRenderer&& other) noexcept;

		virtual ~PhysicsDebugRenderer() override = default;

		FRO_API PhysicsDebugRenderer& operator=(PhysicsDebugRenderer const& other);
		FRO_API PhysicsDebugRenderer& operator=(PhysicsDebugRenderer&& other) noexcept;

		FRO_API virtual void DrawPolygon(b2Vec2 const* const vertices, int32 const vertexCount, b2Color const& color) override;
		FRO_API virtual void DrawSolidPolygon(b2Vec2 const* const vertices, int32 const vertexCount, b2Color const& color) override;
		FRO_API virtual void DrawCircle(b2Vec2 const& center, float const radius, b2Color const& color) override;
		FRO_API virtual void DrawSolidCircle(b2Vec2 const& center, float const radius, b2Vec2 const& axis, b2Color const& color) override;
		FRO_API virtual void DrawSegment(b2Vec2 const& p1, b2Vec2 const& p2, b2Color const& color) override;

		// TODO: what does it mean to draw a transform?
		FRO_API virtual void DrawTransform(b2Transform const&) override;
		FRO_API virtual void DrawPoint(b2Vec2 const& p, float const size, b2Color const& color) override;

		Reference<Renderer const> mRenderer{};
	};
}

#endif