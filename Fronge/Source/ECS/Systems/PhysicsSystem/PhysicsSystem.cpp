#include "froch.hpp"

#include "ECS/Components/Rigidbody/Implementation/RigidbodyImpl.hpp"
#include "Implementation/PhysicsSystemImpl.hpp"
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

		Implementation::sWorld.Step(static_cast<float>(fixedDeltaSeconds), 5, 5);

		for (auto&& [entity, transform, rigidbody] : sGroup)
		{
			transform->setLocalScale({ 1.0, 1.0 });

			auto const translation{ rigidbody->getImplementation().getb2Body().GetPosition() };
			transform->setWorldTranslation({ translation.x, translation.y });

			auto const rotation{ rigidbody->getImplementation().getb2Body().GetAngle() };
			transform->setWorldRotation(rotation);
		}
	}

	Group<Transform, Rigidbody> PhysicsSystem::sGroup{};
}