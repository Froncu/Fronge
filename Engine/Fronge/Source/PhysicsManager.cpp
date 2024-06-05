#include "PhysicsManager.h"

#include "GameObject.h"
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
	b2Body* pBody{ m_World.GetBodyList() };
	for (; pBody; pBody = pBody->GetNext())
	{
		Reference<GameObject> const gameObject
		{
			reinterpret_cast<RigidBody*>(pBody->GetUserData().pointer)->m_ParentingGameObject
		};

		glm::vec2 const translation{ gameObject.get().getWorldTransform().getTranslation() };
		float const rotation{ gameObject.get().getWorldTransform().getRotation() };

		pBody->SetTransform({ translation.x, translation.y }, rotation);
	}

	m_World.Step(fixedDeltaSeconds, 8, 4);

	pBody = m_World.GetBodyList();
	for (; pBody; pBody = pBody->GetNext())
	{
		b2Vec2 const& translation{ pBody->GetPosition() };
		float const rotation{ pBody->GetAngle() };

		Reference<RigidBody> const rigidBody{
			*reinterpret_cast<RigidBody*>(pBody->GetUserData().pointer) };

		rigidBody.get().m_ParentingGameObject.get().setWorldTranslation({ translation.x, translation.y });
		rigidBody.get().m_ParentingGameObject.get().setWorldRotation(rotation);
	}
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::PhysicsManager::BeginContact(b2Contact* const pContact)
{
	Reference<RigidBody> const rigidBodyA{ reinterpret_cast<RigidBody*>(pContact->GetFixtureA()->GetBody()->GetUserData().pointer) };
	Reference<RigidBody> const rigidBodyB{ reinterpret_cast<RigidBody*>(pContact->GetFixtureB()->GetBody()->GetUserData().pointer) };

	rigidBodyA.get().m_sOverlappingRigidBodies.insert(rigidBodyB);
	rigidBodyB.get().m_sOverlappingRigidBodies.insert(rigidBodyA);

	rigidBodyA.get().beginOverlap.notifySubscribers(rigidBodyB);
	rigidBodyB.get().beginOverlap.notifySubscribers(rigidBodyA);
}

void fro::PhysicsManager::EndContact(b2Contact* const pContact)
{
	Reference<RigidBody> const rigidBodyA{ reinterpret_cast<RigidBody*>(pContact->GetFixtureA()->GetBody()->GetUserData().pointer) };
	Reference<RigidBody> const rigidBodyB{ reinterpret_cast<RigidBody*>(pContact->GetFixtureB()->GetBody()->GetUserData().pointer) };

	rigidBodyA.get().m_sOverlappingRigidBodies.erase(rigidBodyB);
	rigidBodyB.get().m_sOverlappingRigidBodies.erase(rigidBodyA);

	rigidBodyA.get().endOverlap.notifySubscribers(rigidBodyB);
	rigidBodyB.get().endOverlap.notifySubscribers(rigidBodyA);
}
#pragma endregion PrivateMethods