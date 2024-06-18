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
	for (std::size_t index1{}; index1 < m_vRigidBodies.size(); ++index1)
	{
		Reference<RigidBody> rigidBody1{ m_vRigidBodies[index1] };

		if (not rigidBody1.get().parentingGameObject.get().isActive())
			continue;

		rigidBody1.get().parentingGameObject.get().worldTranslate(rigidBody1.get().velocity * fixedDeltaSeconds);

		for (std::size_t const idleBodyIndex : m_vIdleBodyIndices)
			updateOverlap(m_vRigidBodies[index1], m_vRigidBodies[idleBodyIndex]);

		for (std::size_t index2{ index1 + 1 }; index2 < m_vRigidBodies.size(); ++index2)
			updateOverlap(m_vRigidBodies[index1], m_vRigidBodies[index2]);
	}

	m_vIdleBodyIndices.clear();
}

void fro::PhysicsManager::registerRigidBody(Reference<RigidBody> const rigidBody)
{
	m_vRigidBodies.push_back(rigidBody);
}

void fro::PhysicsManager::unregisterRigidBody(Reference<RigidBody> const rigidBody)
{
	auto const iRigidBodyToUnregister
	{
		std::find_if(m_vRigidBodies.begin(), m_vRigidBodies.end(),
			[rigidBody](Reference<RigidBody> const& otherRigidBody)
			{
				return rigidBody == otherRigidBody;
			})
	};

	if (iRigidBodyToUnregister not_eq m_vRigidBodies.end())
		m_vRigidBodies.erase(iRigidBodyToUnregister);
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::PhysicsManager::updateOverlap(Reference<RigidBody> const body1, Reference<RigidBody> const body2)
{
	if (not body2.get().parentingGameObject.get().isActive())
		return;

	if (body1 == body2)
		return;

	bool const contains{ body1.get().m_sOverlappingBodies.contains(body2) };
	bool const overlapping{ areOverlapping(body1, body2) };

	if (overlapping and not contains)
	{
		body1.get().m_sOverlappingBodies.insert(body2);
		body2.get().m_sOverlappingBodies.insert(body1);

		beginOverlap.notifySubscribers(body1, body2);

		body1.get().beginOverlap.notifySubscribers(body2);
		body2.get().beginOverlap.notifySubscribers(body1);
	}
	else if (not overlapping and contains)
	{
		body1.get().m_sOverlappingBodies.erase(body2);
		body2.get().m_sOverlappingBodies.erase(body1);

		endOverlap.notifySubscribers(body1, body2);

		body1.get().endOverlap.notifySubscribers(body2);
		body2.get().endOverlap.notifySubscribers(body1);
	}
}

bool fro::PhysicsManager::areOverlapping(Reference<RigidBody> const body1, Reference<RigidBody> const body2)
{
	glm::vec2 const halfSize1{ body1.get().getColliderSize() * 0.5f };
	if (not halfSize1.x and not halfSize1.y)
		return false;

	glm::vec2 const halfSize2{ body2.get().getColliderSize() * 0.5f };
	if (not halfSize2.x and not halfSize2.y)
		return false;

	glm::vec2 const translation1{
		body1.get().parentingGameObject.get().getWorldTransform().getTranslation() };
	glm::vec2 const translation2{
		body2.get().parentingGameObject.get().getWorldTransform().getTranslation() };

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