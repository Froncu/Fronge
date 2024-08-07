#include "froch.hpp"

#include "PhysicsSystemImpl.hpp"
#include "PhysicsDebugRenderer.hpp"

namespace fro
{
	std::unique_ptr<PhysicsDebugRenderer> PhysicsSystem::Implementation::initializeDebugRenderer()
	{
		auto debugRenderer{ std::make_unique<PhysicsDebugRenderer>() };
		PhysicsSystem::Implementation::sWorld.SetDebugDraw(debugRenderer.get());
		return debugRenderer;
	}

	b2World PhysicsSystem::Implementation::sWorld{ b2Vec2{ 0.0f, 9.81f } };
	std::unique_ptr<PhysicsDebugRenderer> const PhysicsSystem::Implementation::sDebugRenderer{ initializeDebugRenderer() };
}