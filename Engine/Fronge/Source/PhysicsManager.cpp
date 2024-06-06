#include "PhysicsManager.h"

#include "GameObject.h"
#include "RigidBody.h"

#include <algorithm>
#include <ranges>

#pragma region Constructors/Destructor
fro::PhysicsManager::PhysicsManager()
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::PhysicsManager::update(float const fixedDeltaSeconds)
{
	for (auto&& [rigidBody1, sOverlappingBodies1, didCheck1] : m_vRigidBodyTuples)
	{
		glm::vec2 const velocity{ rigidBody1.get().velocity };
		if (not velocity.x and not velocity.y)
		{
			didCheck1 = false;
			continue;
		}

		rigidBody1.get().m_ParentingGameObject.get().worldTranslate(
			velocity * fixedDeltaSeconds);

		for (auto&& [rigidBody2, sOverlappingBodies2, didCheck2] : m_vRigidBodyTuples)
		{
			if (didCheck2)
				continue;

			if (rigidBody1 == rigidBody2)
				continue;

			bool const contains{ sOverlappingBodies1.contains(rigidBody2) };
			bool const overlapping{ areOverlapping(rigidBody1, rigidBody2) };

			if (overlapping and not contains)
			{
				sOverlappingBodies1.insert(rigidBody2);
				sOverlappingBodies2.insert(rigidBody1);

				beginOverlap.notifySubscribers(rigidBody1, rigidBody2);
			}
			else if (not overlapping and contains)
			{
				sOverlappingBodies1.erase(rigidBody2);
				sOverlappingBodies2.erase(rigidBody1);

				endOverlap.notifySubscribers(rigidBody1, rigidBody2);
			}
		}

		didCheck1 = true;
	}
}

void fro::PhysicsManager::registerRigidBody(Reference<RigidBody> const rigidBody)
{
	m_vRigidBodyTuples.push_back({ rigidBody, {}, {} });
}

void fro::PhysicsManager::unregisterRigidBody(Reference<RigidBody> const rigidBody)
{
	auto const iNewEnd
	{
		std::remove_if(m_vRigidBodyTuples.begin(), m_vRigidBodyTuples.end(),
			[rigidBody](RigidBodyTuple const rigidBodyTuple)
			{
				return rigidBody == std::get<0>(rigidBodyTuple);
			})
	};

	if (iNewEnd not_eq m_vRigidBodyTuples.end())
		m_vRigidBodyTuples.erase(iNewEnd, m_vRigidBodyTuples.end());
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
bool fro::PhysicsManager::areOverlapping(Reference<RigidBody> const body1, Reference<RigidBody> const body2)
{
	glm::vec2 const halfSize1{ body1.get().getColliderSize() * 0.5f };
	if (not halfSize1.x and not halfSize1.y)
		return false;

	glm::vec2 const halfSize2{ body2.get().getColliderSize() * 0.5f };
	if (not halfSize2.x and not halfSize2.y)
		return false;

	glm::vec2 const translation1{
		body1.get().m_ParentingGameObject.get().getWorldTransform().getTranslation() };
	glm::vec2 const translation2{
		body2.get().m_ParentingGameObject.get().getWorldTransform().getTranslation() };

	glm::vec2 const topLeft1{ translation1 - halfSize1 };
	glm::vec2 const bottomRight1{ translation1 + halfSize1 };
	glm::vec2 const topLeft2{ translation2 - halfSize2 };
	glm::vec2 const bottomRight2{ translation2 + halfSize2 };

	return
		not (topLeft1.x >= bottomRight2.x) and
		not (bottomRight1.x <= topLeft2.x) and
		not (topLeft1.y >= bottomRight2.y) and
		not (bottomRight1.y <= topLeft2.y);
}
#pragma endregion PrivateMethods