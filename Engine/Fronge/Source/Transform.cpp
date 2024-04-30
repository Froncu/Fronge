#include "Transform.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Transform::Transform(GameObject const& parentingGameObject) :
	Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Transform::localTranslate(glm::vec2 const& localTranslator)
{
	getLocalPositionInternal() += localTranslator;

	setWorldPositionDirty();
}

void fro::Transform::worldTranslate(glm::vec2 const& worldTranslator)
{
	getWorldPositionInternal() += worldTranslator;

	setLocalPositionDirty();
}

void fro::Transform::setLocalPosition(glm::vec2 const& localPosition)
{
	m_LocalPosition = localPosition;
	m_IsLocalPositionDirty = false;

	setWorldPositionDirty();
}

void fro::Transform::setWorldPosition(glm::vec2 const& worldPosition)
{
	m_WorldPosition = worldPosition;
	m_IsWorldPositionDirty = false;

	setLocalPositionDirty();
}

glm::vec2 const& fro::Transform::getLocalPosition()
{
	return getLocalPositionInternal();
}

glm::vec2 const& fro::Transform::getWorldPosition()
{
	return getWorldPositionInternal();
}

bool fro::Transform::isLocalPositionDirty() const
{
	return m_IsLocalPositionDirty;
}

bool fro::Transform::isWorldPositionDirty() const
{
	return m_IsWorldPositionDirty;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Transform::calculateLocalPosition()
{
	GameObject const* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
	if (pParentingGameObjectsParent)
		m_LocalPosition = getWorldPosition() - pParentingGameObjectsParent->getComponent<Transform>()->getWorldPosition();
	else
		m_LocalPosition = getWorldPosition();
}

void fro::Transform::calculateWorldPosition()
{
	GameObject const* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
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

	for (GameObject const* const pChild : getParentingGameObject().getChildren())
	{
		Transform& childTransform{ *pChild->getComponent<Transform>() };
		if (not childTransform.isWorldPositionDirty())
			childTransform.setWorldPositionDirty();
	}
}

void fro::Transform::setWorldPositionDirty()
{
	if (m_IsLocalPositionDirty)
	{
		calculateLocalPosition();
		m_IsLocalPositionDirty = false;
	}

	m_IsWorldPositionDirty = true;

	for (GameObject const* const pChild : getParentingGameObject().getChildren())
	{
		Transform& childTransform{ *pChild->getComponent<Transform>() };
		if (not childTransform.isWorldPositionDirty())
			childTransform.setWorldPositionDirty();
	}
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