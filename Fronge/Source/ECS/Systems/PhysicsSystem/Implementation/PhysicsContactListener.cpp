#include "froch.hpp"

#include "ECS/Components/Rigidbody/Rigidbody.hpp"
#include "ECS/Systems/PhysicsSystem/PhysicsSystem.hpp"
#include "PhysicsContactListener.h"

#include <box2d/b2_contact.h>

namespace fro
{
	void PhysicsContactListener::BeginContact(b2Contact* const contact)
	{
		auto const userDataFixtureA{ contact->GetFixtureA()->GetUserData().pointer };
		Collider& colliderA{ *reinterpret_cast<Collider* const>(userDataFixtureA) };

		auto const userDataFixtureB{ contact->GetFixtureB()->GetUserData().pointer };
		Collider& colliderB{ *reinterpret_cast<Collider* const>(userDataFixtureB) };

		auto const userDataBodyA{ contact->GetFixtureA()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyA{ *reinterpret_cast<Rigidbody* const>(userDataBodyA) };

		auto const userDataBodyB{ contact->GetFixtureB()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyB{ *reinterpret_cast<Rigidbody* const>(userDataBodyB) };

		PhysicsSystem::sBeginContactEvent.notify(rigibodyA, rigibodyB);
		rigibodyA.mBeginContactEvent.notify(colliderA, rigibodyB, colliderB);
		rigibodyB.mBeginContactEvent.notify(colliderB, rigibodyA, colliderA);
	}

	void PhysicsContactListener::EndContact(b2Contact* const contact)
	{
		auto const userDataFixtureA{ contact->GetFixtureA()->GetUserData().pointer };
		Collider& colliderA{ *reinterpret_cast<Collider* const>(userDataFixtureA) };

		auto const userDataFixtureB{ contact->GetFixtureB()->GetUserData().pointer };
		Collider& colliderB{ *reinterpret_cast<Collider* const>(userDataFixtureB) };

		auto const userDataBodyA{ contact->GetFixtureA()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyA{ *reinterpret_cast<Rigidbody* const>(userDataBodyA) };

		auto const userDataBodyB{ contact->GetFixtureB()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyB{ *reinterpret_cast<Rigidbody* const>(userDataBodyB) };

		PhysicsSystem::sEndContactEvent.notify(rigibodyA, rigibodyB);
		rigibodyA.mEndContactEvent.notify(colliderA, rigibodyB, colliderB);
		rigibodyB.mEndContactEvent.notify(colliderB, rigibodyA, colliderA);
	}
}