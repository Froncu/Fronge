#include "RigidBody.h"

#include "Console.hpp"
#include "GameObject.h"
#include "PhysicsManager.h"

#include <algorithm>
#include <functional>

#pragma region Constructors/Destructor
fro::RigidBody::RigidBody(Reference<GameObject> const parentingGameObject)
	: Component(parentingGameObject)
{
	PhysicsManager::getInstance().registerRigidBody(this);

	parentingGameObject.get().activeStateChanged.addSubscriber(
		std::bind(&RigidBody::onActiveStateChanged, this, std::placeholders::_1));
}

fro::RigidBody::~RigidBody()
{
	PhysicsManager::getInstance().unregisterRigidBody(this);
}
#pragma endregion Constructors/Destructor



#pragma region Operators
bool fro::RigidBody::RigidBodyComparer::operator()
(Reference<RigidBody> const body1, Reference<RigidBody> const body2) const
{
	return body1 < body2;
}

bool fro::RigidBody::RigidBodyComparer::operator()
(Reference<RigidBody> const body, std::string_view const tag) const
{
	return body.get().parentingGameObject.get().getTag() < tag;
}

bool fro::RigidBody::RigidBodyComparer::operator()
(std::string_view const tag, Reference<RigidBody> const body) const
{
	return body.get().parentingGameObject.get().getTag() < tag;
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::RigidBody::setColliderSize(glm::vec2 const size)
{
	m_ColliderSize.x = std::max(size.x, 0.0f);
	m_ColliderSize.y = std::max(size.y, 0.0f);
}

fro_NODISCARD glm::vec2 fro::RigidBody::getColliderSize() const
{
	return m_ColliderSize;
}

bool fro::RigidBody::isOverlapping(Reference<RigidBody> const other) const
{
	return m_sOverlappingBodies.contains(other);
}

bool fro::RigidBody::isOverlapping(std::string_view const tag) const
{
	return m_sOverlappingBodies.contains(tag);
}

fro::Reference<fro::RigidBody> fro::RigidBody::getOverlapping(std::string_view const tag) const
{
	auto const iFoundOverlappingBody{ m_sOverlappingBodies.find(tag) };
	if (iFoundOverlappingBody == m_sOverlappingBodies.end())
		return {};

	return *iFoundOverlappingBody;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::RigidBody::onActiveStateChanged(bool const isActive)
{
	if (isActive)
		return;

	auto iOverlappingBody{ m_sOverlappingBodies.begin() };
	while (iOverlappingBody not_eq m_sOverlappingBodies.end())
	{
		Reference<RigidBody> const overlappingBody{ *iOverlappingBody };

		m_sOverlappingBodies.erase(iOverlappingBody);
		overlappingBody.get().m_sOverlappingBodies.erase(this);

		PhysicsManager::getInstance().endOverlap.notifySubscribers(this, overlappingBody);

		endOverlap.notifySubscribers(overlappingBody);
		overlappingBody.get().endOverlap.notifySubscribers(this);

		iOverlappingBody = m_sOverlappingBodies.begin();
	}
}
#pragma endregion PrivateMethods