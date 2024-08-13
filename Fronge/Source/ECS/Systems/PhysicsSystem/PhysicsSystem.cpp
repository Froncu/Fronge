#include "froch.hpp"

#include "ECS/Components/Rigidbody/Implementation/RigidbodyImpl.hpp"
#include "Implementation/PhysicsSystemImpl.hpp"
#include "Implementation/PhysicsDebugRenderer.hpp"
#include "PhysicsSystem.hpp"

#include <box2d/b2_fixture.h>

namespace fro
{
	void PhysicsSystem::onFixedUpdate(double const fixedDeltaSeconds)
	{
		for (auto&& [entity, transform, rigidbody] : sGroup)
		{
			auto const translation{ transform->world().getTranslation() };
			auto const rotation{ transform->world().getRotation() };

			rigidbody->getImplementation().getb2Body().SetTransform
			(
				{ static_cast<float>(translation.x), static_cast<float>(translation.y) },
				static_cast<float>(rotation)
			);
		}

		Implementation::sWorld.Step(static_cast<float>(fixedDeltaSeconds), sVelocityIterations, sPositionIterations);

		for (auto&& [entity, transform, rigidbody] : sGroup)
		{
			transform->setLocalScale({ 1.0, 1.0 });

			auto const translation{ rigidbody->getImplementation().getb2Body().GetPosition() };
			transform->setWorldTranslation({ translation.x, translation.y });

			auto const rotation{ rigidbody->getImplementation().getb2Body().GetAngle() };
			transform->setWorldRotation(rotation);
		}
	}

	void PhysicsSystem::onRender(Renderer const& renderer)
	{
		if (auto& currentRenderer{ Implementation::sDebugRenderer.mRenderer };
			not currentRenderer.valid() or &*currentRenderer not_eq &renderer)
			Implementation::sDebugRenderer.mRenderer = renderer;

		Implementation::sWorld.DebugDraw();
	}

	void PhysicsSystem::setGravity(Vector2<double> const gravity)
	{
		Implementation::sWorld.SetGravity({ static_cast<float>(gravity.x), static_cast<float>(gravity.y) });
	}

	Reference<Rigidbody> PhysicsSystem::raycast(Vector2<double> const from, Vector2<double> const to)
	{
		b2RayCastInput const input
		{
			.p1{ static_cast<float>(from.x), static_cast<float>(from.y) },
			.p2{ static_cast<float>(to.x), static_cast<float>(to.y) },
			.maxFraction{ 1.0f }
		};

		float closestFraction{ 1.0f };

		b2Body* closestBody{};
		for (b2Body* body{ Implementation::sWorld.GetBodyList() }; body; body = body->GetNext())
			for (b2Fixture* fixture{ body->GetFixtureList() }; fixture; fixture = fixture->GetNext())
			{
				b2RayCastOutput output;
				if (not fixture->RayCast(&output, input, 0))
					continue;

				if (output.fraction < closestFraction)
				{
					closestFraction = output.fraction;
					closestBody = body;
				}
			}

		if (not closestBody)
			return {};

		return reinterpret_cast<Rigidbody*>(closestBody->GetUserData().pointer);
	}

	EventDispatcher<Rigidbody, Rigidbody> PhysicsSystem::sBeginContactEvent{};
	EventDispatcher<Rigidbody, Rigidbody> PhysicsSystem::sEndContactEvent{};
	std::int32_t PhysicsSystem::sVelocityIterations{ 8 };
	std::int32_t PhysicsSystem::sPositionIterations{ 3 };
	Group<Transform, Rigidbody> PhysicsSystem::sGroup{};
}