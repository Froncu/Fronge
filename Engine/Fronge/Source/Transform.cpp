#include "Transform.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Transform::Transform(GameObject const& parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Transform::localTransform(TransformationMatrix2D const& transformation)
{
	m_LocalTransform.transform(transformation.getTransformation());

	setWorldTransformDirty();
}

void fro::Transform::localTranslate(glm::vec2 const& translation)
{
	m_LocalTransform.translate(translation);

	setWorldTransformDirty();
}

void fro::Transform::localRotate(float const rotation)
{
	m_LocalTransform.rotate(rotation);

	setWorldTransformDirty();
}

void fro::Transform::localScale(glm::vec2 const& scale)
{
	m_LocalTransform.scale(scale);

	setWorldTransformDirty();
}

void fro::Transform::worldTransform(TransformationMatrix2D const& transformation)
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.transform(transformation.getTransformation());

	calculateLocalTransform();
}

void fro::Transform::worldTranslate(glm::vec2 const& translation)
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.translate(translation);

	calculateLocalTransform();
}

void fro::Transform::worldRotate(float const rotation)
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.rotate(rotation);

	calculateLocalTransform();
}

void fro::Transform::worldScale(glm::vec2 const& scale)
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.scale(scale);

	calculateLocalTransform();
}

void fro::Transform::setLocalTransformation(TransformationMatrix2D const& transformation)
{
	m_LocalTransform = transformation;

	setWorldTransformDirty();
}

void fro::Transform::setLocalTranslation(glm::vec2 const& translation)
{
	m_LocalTransform.setTranslation(translation);

	setWorldTransformDirty();
}

void fro::Transform::setLocalRotation(float const rotation)
{
	m_LocalTransform.setRotation(rotation);

	setWorldTransformDirty();
}

void fro::Transform::setLocalScale(glm::vec2 const& scale)
{
	m_LocalTransform.setScale(scale);

	setWorldTransformDirty();
}

void fro::Transform::setWorldTransformDirty()
{
	m_IsWorldTransformDirty = true;

	for (GameObject const* const pChild : getParentingGameObject().getChildren())
	{
		Transform& childTransform{ *pChild->getComponent<Transform>() };
		if (not childTransform.m_IsWorldTransformDirty)
			childTransform.setWorldTransformDirty();
	}
}

void fro::Transform::setWorldTransformation(TransformationMatrix2D const& transformation)
{
	m_WorldTransform = transformation;
	m_IsWorldTransformDirty = false;

	calculateLocalTransform();
}

void fro::Transform::setWorldTranslation(glm::vec2 const& translation)
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.setTranslation(translation);

	calculateLocalTransform();
}

void fro::Transform::setWorldRotation(float const rotation)
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform(); 
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.setRotation(rotation);

	calculateLocalTransform();
}

void fro::Transform::setWorldScale(glm::vec2 const& scale)
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.setScale(scale);

	calculateLocalTransform();
}

fro::TransformationMatrix2D const& fro::Transform::getLocalTransform()
{
	return m_LocalTransform;
}

fro::TransformationMatrix2D const& fro::Transform::getWorldTransform()
{
	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	return m_WorldTransform;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Transform::calculateLocalTransform()
{
	GameObject const* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
	if (pParentingGameObjectsParent)
		m_LocalTransform = getWorldTransform() / pParentingGameObjectsParent->getComponent<Transform>()->getWorldTransform();
	else
		m_LocalTransform = getWorldTransform();
}

void fro::Transform::calculateWorldTransform()
{
	GameObject const* const pParentingGameObjectsParent{ getParentingGameObject().getParent() };
	if (pParentingGameObjectsParent)
		m_WorldTransform = getLocalTransform() * pParentingGameObjectsParent->getComponent<Transform>()->getWorldTransform();
	else
		m_WorldTransform = getLocalTransform();
}
#pragma endregion PrivateMethods