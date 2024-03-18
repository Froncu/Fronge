#include "Transform.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro_GENERATED_COMPONENT_CONSTRUCTOR(Transform)
fro_GENERATED_COMPONENT_DESTRUCTOR(Transform)
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Transform::localTranslate(const glm::vec2& localTranslator)
{
	getLocalPositionInternal() += localTranslator;

	setWorldPositionDirty();
}

void fro::Transform::worldTranslate(const glm::vec2& worldTranslator)
{
	getWorldPositionInternal() += worldTranslator;

	setLocalPositionDirty();
}

void fro::Transform::setLocalPosition(const glm::vec2& localPosition)
{
	m_LocalPosition = localPosition;
	m_IsLocalPositionDirty = false;

	setWorldPositionDirty();
}

void fro::Transform::setWorldPosition(const glm::vec2& worldPosition)
{
	m_WorldPosition = worldPosition;
	m_IsWorldPositionDirty = false;

	setLocalPositionDirty();
}

const glm::vec2& fro::Transform::getLocalPosition()
{
	return getLocalPositionInternal();
}

const glm::vec2& fro::Transform::getWorldPosition()
{
	return getWorldPositionInternal();
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Transform::calculateLocalPosition()
{
	const GameObject* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
	if (pParentingGameObjectsParent)
		m_LocalPosition = getWorldPosition() - pParentingGameObjectsParent->getComponent<Transform>()->getWorldPosition();
	else
		m_LocalPosition = getWorldPosition();
}

void fro::Transform::calculateWorldPosition()
{
	const GameObject* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
	if (pParentingGameObjectsParent)
		m_WorldPosition = getLocalPosition() + pParentingGameObjectsParent->getComponent<Transform>()->getWorldPosition();
	else
		m_WorldPosition = getLocalPosition();
}

void fro::Transform::setLocalPositionDirty()
{
	if (m_IsWorldPositionDirty)
	{
		calculateWorldPosition();
		m_IsLocalPositionDirty = false;
	}

	m_IsLocalPositionDirty = true;

	for (const GameObject* const pChild : getParentingGameObject().getChildren())
		pChild->getComponent<Transform>()->setWorldPositionDirty();
}

void fro::Transform::setWorldPositionDirty()
{
	if (m_IsLocalPositionDirty)
	{
		calculateLocalPosition();
		m_IsLocalPositionDirty = false;
	}

	m_IsWorldPositionDirty = true;

	for (const GameObject* const pChild : getParentingGameObject().getChildren())
		pChild->getComponent<Transform>()->setWorldPositionDirty();
}

glm::vec2& fro::Transform::getLocalPositionInternal()
{
	if (m_IsLocalPositionDirty)
	{
		calculateLocalPosition();
		m_IsLocalPositionDirty = false;
	}

	return m_LocalPosition;
}

glm::vec2& fro::Transform::getWorldPositionInternal()
{
	if (m_IsWorldPositionDirty)
	{
		calculateWorldPosition();
		m_IsWorldPositionDirty = false;
	}

	return m_WorldPosition;
}
#pragma endregion PrivateMethods