#include "froch.hpp"

#include "ECS/Components/Rigidbody/Implementation/RigidbodyImpl.hpp"
#include "Implementation/PhysicsSystemImpl.hpp"
#include "Implementation/PhysicsDebugRenderer.hpp"
#include "PhysicsSystem.hpp"

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
		if (auto& currentRenderer{ Implementation::sDebugRenderer->mRenderer };
			not currentRenderer.valid() or &*currentRenderer not_eq &renderer)
			Implementation::sDebugRenderer->mRenderer = renderer;

		Implementation::sWorld.DebugDraw();
	}

	void PhysicsSystem::setGravity(Vector2<double> const gravity)
	{
		Implementation::sWorld.SetGravity({ static_cast<float>(gravity.x), static_cast<float>(gravity.y) });
	}

	std::int32_t PhysicsSystem::sVelocityIterations{ 8 };
	std::int32_t PhysicsSystem::sPositionIterations{ 3 };
	Group<Transform, Rigidbody> PhysicsSystem::sGroup{};
}