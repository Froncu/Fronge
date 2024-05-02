#include "Transform.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Transform::Transform(GameObject const& parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Transform::setLocalTransformation(Matrix2D const& transformation)
{
	m_LocalTransform = transformation;

	setWorldTransformDirty();
}

void fro::Transform::setLocalTranslation(glm::vec2 const& translation)
{
	calculateLocalTransform();
	m_LocalTransform.setTranslation(translation);

	setWorldTransformDirty();
}

void fro::Transform::setLocalRotation(float const rotation)
{
	calculateLocalTransform();
	m_LocalTransform.setRotation(rotation);

	setWorldTransformDirty();
}

void fro::Transform::setLocalScale(glm::vec2 const& scale)
{
	calculateLocalTransform();
	m_LocalTransform.setScale(scale);

	setWorldTransformDirty();
}

void fro::Transform::setWorldTransformDirty()
{
	calculateLocalTransform();
	m_IsWorldTransformDirty = true;

	for (GameObject const* const pChild : getParentingGameObject().getChildren())
	{
		Transform& childTransform{ *pChild->getComponent<Transform>() };
		if (not childTransform.m_IsWorldTransformDirty)
			childTransform.setWorldTransformDirty();
	}
}

void fro::Transform::setWorldTransformation(Matrix2D const& transformation)
{
	m_WorldTransform = transformation;

	setLocalTransformDirty();
}

void fro::Transform::setWorldTranslation(glm::vec2 const& translation)
{
	calculateWorldTransform();
	m_WorldTransform.setTranslation(translation);

	setLocalTransformDirty();
}

void fro::Transform::setWorldRotation(float const rotation)
{
	calculateWorldTransform();
	m_WorldTransform.setRotation(rotation);

	setLocalTransformDirty();
}

void fro::Transform::setWorldScale(glm::vec2 const& scale)
{
	calculateWorldTransform();
	m_WorldTransform.setScale(scale);;

	setLocalTransformDirty();
}

fro::Matrix2D const& fro::Transform::getLocalTransform()
{
	calculateLocalTransform();
	return m_LocalTransform;
}

fro::Matrix2D const& fro::Transform::getWorldTransform()
{
	calculateWorldTransform();
	return m_WorldTransform;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Transform::setLocalTransformDirty()
{
	calculateWorldTransform();
	m_IsLocalTransformDirty = true;

	for (GameObject const* const pChild : getParentingGameObject().getChildren())
	{
		Transform& childTransform{ *pChild->getComponent<Transform>() };
		if (not childTransform.m_IsWorldTransformDirty)
			childTransform.setWorldTransformDirty();
	}
}

void fro::Transform::calculateLocalTransform()
{
	if (not m_IsLocalTransformDirty)
		return;

	GameObject const* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
	if (pParentingGameObjectsParent)
		m_LocalTransform = pParentingGameObjectsParent->getComponent<Transform>()->getWorldTransform() / getWorldTransform();
	else
		m_LocalTransform = getWorldTransform();

	m_IsLocalTransformDirty = false;
}

void fro::Transform::calculateWorldTransform()
{
	if (not m_IsWorldTransformDirty)
		return;

	GameObject const* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
	if (pParentingGameObjectsParent)
		m_WorldTransform = getLocalTransform() * pParentingGameObjectsParent->getComponent<Transform>()->getWorldTransform();
	else
		m_WorldTransform = getLocalTransform();

	m_IsWorldTransformDirty = false;
}
#pragma endregion PrivateMethods