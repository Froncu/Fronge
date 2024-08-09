#include "froch.hpp"

#include "PhysicsSystemImpl.hpp"

namespace fro
{
	PhysicsDebugRenderer PhysicsSystem::Implementation::createDebugRenderer()
	{
		PhysicsDebugRenderer debugRenderer{};

		debugRenderer.AppendFlags(b2Draw::e_shapeBit);

		sWorld.SetDebugDraw(&sDebugRenderer);
		return debugRenderer;
	}

	PhysicsContactListener PhysicsSystem::Implementation::createContactListener()
	{
		PhysicsContactListener contactListener{};

		sWorld.SetContactListener(&sContactListener);
		return contactListener;
	}

	b2World PhysicsSystem::Implementation::sWorld{ b2Vec2{ 0.0f, 9.81f } };
	PhysicsDebugRenderer PhysicsSystem::Implementation::sDebugRenderer{ createDebugRenderer() };
	PhysicsContactListener PhysicsSystem::Implementation::sContactListener{ createContactListener() };
}