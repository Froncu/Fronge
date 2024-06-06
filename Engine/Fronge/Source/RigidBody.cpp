#include "RigidBody.h"

#include "GameObject.h"
#include "PhysicsManager.h"

#pragma region Constructors/Destructor
fro::RigidBody::RigidBody(Reference<GameObject> const parentingGameObject)
	: Component(parentingGameObject)
{
	PhysicsManager::getInstance().registerRigidBody(this);
}

fro::RigidBody::~RigidBody()
{
	PhysicsManager::getInstance().unregisterRigidBody(this);
}
#pragma endregion Constructors/Destructor



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
#pragma endregion PublicMethods