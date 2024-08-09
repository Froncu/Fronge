#include "froch.hpp"

#include "ECS/Components/Rigidbody/Rigidbody.hpp"
#include "ECS/Systems/PhysicsSystem/PhysicsSystem.hpp"
#include "PhysicsContactListener.h"

#include <box2d/b2_contact.h>

namespace fro
{
	void PhysicsContactListener::BeginContact(b2Contact* const contact)
	{
		auto const userDataA{ contact->GetFixtureA()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyA{ *reinterpret_cast<Rigidbody* const>(userDataA) };
		auto const userDataB{ contact->GetFixtureB()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyB{ *reinterpret_cast<Rigidbody* const>(userDataB) };

		PhysicsSystem::sBeginContactEvent.notify(rigibodyA, rigibodyB);
		rigibodyA.mBeginContactEvent.notify(rigibodyB);
		rigibodyB.mBeginContactEvent.notify(rigibodyA);
	}

	void PhysicsContactListener::EndContact(b2Contact* const contact)
	{
		auto const userDataA{ contact->GetFixtureA()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyA{ *reinterpret_cast<Rigidbody* const>(userDataA) };
		auto const userDataB{ contact->GetFixtureB()->GetBody()->GetUserData().pointer };
		Rigidbody& rigibodyB{ *reinterpret_cast<Rigidbody* const>(userDataB) };

		PhysicsSystem::sEndContactEvent.notify(rigibodyA, rigibodyB);
		rigibodyA.mEndContactEvent.notify(rigibodyB);
		rigibodyB.mEndContactEvent.notify(rigibodyA);
	}
}