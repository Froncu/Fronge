#include "PhysicsManager.h"

#include "RigidBody.h"

#include <b2_contact.h>

#pragma region Constructors/Destructor
fro::PhysicsManager::PhysicsManager()
{
	m_World.SetContactListener(this);
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::PhysicsManager::update(float const fixedDeltaSeconds)
{
	m_World.Step(fixedDeltaSeconds, 6, 2);
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::PhysicsManager::BeginContact(b2Contact* const pContact)
{
	RigidBody& rigidBodyA{ *reinterpret_cast<RigidBody*>(pContact->GetFixtureA()->GetBody()->GetUserData().pointer) };
	RigidBody& rigidBodyB{ *reinterpret_cast<RigidBody*>(pContact->GetFixtureB()->GetBody()->GetUserData().pointer) };

	rigidBodyA.m_spOverlappingRigidBodies.insert(&rigidBodyB);
	rigidBodyB.m_spOverlappingRigidBodies.insert(&rigidBodyA);
}

void fro::PhysicsManager::EndContact(b2Contact* const pContact)
{
	RigidBody& rigidBodyA{ *reinterpret_cast<RigidBody*>(pContact->GetFixtureA()->GetBody()->GetUserData().pointer) };
	RigidBody& rigidBodyB{ *reinterpret_cast<RigidBody*>(pContact->GetFixtureB()->GetBody()->GetUserData().pointer) };

	rigidBodyA.m_spOverlappingRigidBodies.erase(&rigidBodyB);
	rigidBodyB.m_spOverlappingRigidBodies.erase(&rigidBodyA);
}
#pragma endregion PrivateMethods