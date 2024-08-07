#if not defined PHYSICS_DEBUG_RENDERER_HPP
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

		virtual ~PhysicsDebugRenderer() override = default;

		FRO_API virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
		FRO_API virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
		FRO_API virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
		FRO_API virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
		FRO_API virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
		FRO_API virtual void DrawTransform(const b2Transform& xf) override;
		FRO_API virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

		Reference<Renderer const> mRenderer{};

	private:
		PhysicsDebugRenderer(PhysicsDebugRenderer const&) = delete;
		PhysicsDebugRenderer(PhysicsDebugRenderer&&) noexcept = delete;

		PhysicsDebugRenderer& operator=(PhysicsDebugRenderer const&) = delete;
		PhysicsDebugRenderer& operator=(PhysicsDebugRenderer&&) noexcept = delete;
	};
}

#endif