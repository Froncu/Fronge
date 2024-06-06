#include "GameObject.h"

#include <algorithm>

#pragma region Constructors/Destructor
fro::GameObject::GameObject(std::string name, std::string tag)
	: m_Name{ std::move(name) }
	, m_Tag{ std::move(tag) }
{
}

fro::GameObject::GameObject(GameObject&& other) noexcept
	: BaseReferencable(std::move(other))

	, m_Name{ std::move(other.m_Name) }
	, m_Tag{ std::move(other.m_Tag) }
	
	, m_mpComponents{ std::move(other.m_mpComponents) }

	, m_vBehaviours{ std::move(other.m_vBehaviours) }
	, m_vRenderables{ std::move(other.m_vRenderables) }
	, m_vGUIs{ std::move(other.m_vGUIs) }

	, m_vChildren{ std::move(other.m_vChildren) }
	, m_Parent{ std::move(other.m_Parent) }

	, m_LocalTransform{ std::move(other.m_LocalTransform) }
	, m_WorldTransform{ std::move(other.m_WorldTransform) }

	, m_IsWorldTransformDirty{ other.m_IsWorldTransformDirty }

	, m_IsActive{ other.m_IsActive }
{
	other.m_IsWorldTransformDirty = false;
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::GameObject& fro::GameObject::operator=(GameObject&& other) noexcept
{
	BaseReferencable::operator=(std::move(other));

	m_mpComponents = std::move(other.m_mpComponents);

	m_vBehaviours = std::move(other.m_vBehaviours);
	m_vRenderables = std::move(other.m_vRenderables);
	m_vGUIs = std::move(other.m_vGUIs);

	m_vChildren = std::move(other.m_vChildren);
	m_Parent = std::move(other.m_Parent);

	m_IsActive = other.m_IsActive;

	return *this;
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::GameObject::fixedUpdate(float const fixedDeltaSeconds) const
{
	if (m_IsActive)
		for (Reference<Behaviour> const behaviour : m_vBehaviours)
			behaviour.get().fixedUpdate(fixedDeltaSeconds);
}

void fro::GameObject::update(float const deltaSeconds) const
{
	if (m_IsActive)
		for (Reference<Behaviour> const behaviour : m_vBehaviours)
			behaviour.get().update(deltaSeconds);
}

void fro::GameObject::lateUpdate(float const deltaSeconds) const
{
	if (m_IsActive)
		for (Reference<Behaviour> const behaviour : m_vBehaviours)
			behaviour.get().lateUpdate(deltaSeconds);
}

void fro::GameObject::render() const
{
	if (m_IsActive)
		for (Reference<Renderable> const renderable : m_vRenderables)
			renderable.get().render();
}

void fro::GameObject::display() const
{
	if (m_IsActive)
		for (Reference<GUI> const GUI : m_vGUIs)
			GUI.get().display();
}

void fro::GameObject::setActive(bool const isActive)
{
	m_IsActive = isActive;
}

void fro::GameObject::setParent(Reference<GameObject> const parent, bool const keepWorldTransform)
{
	if (parent == this || parent == m_Parent || owns(parent))
		return;

	if (m_Parent.valid())
		m_vChildren.erase(
			std::remove(m_vChildren.begin(), m_vChildren.end(), this),
			m_vChildren.end());

	TransformationMatrix2D oldWorldTransform;
	if (keepWorldTransform)
		oldWorldTransform = getWorldTransform();

	m_Parent = parent;

	if (m_Parent.valid())
		m_Parent.get().m_vChildren.push_back(this);

	if (m_Parent.valid() and keepWorldTransform)
		setWorldTransformation(oldWorldTransform);
	else
		setWorldTransformDirty();
}

void fro::GameObject::localTransform(TransformationMatrix2D const& transformation)
{
	if (transformation.getTransformation() == glm::mat3x3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f))
		return;

	m_LocalTransform.transform(transformation.getTransformation());

	setWorldTransformDirty();
}

void fro::GameObject::localTranslate(glm::vec2 const& translation)
{
	if (not translation.x and not translation.y)
		return;

	m_LocalTransform.translate(translation);

	setWorldTransformDirty();
}

void fro::GameObject::localRotate(float const rotation)
{
	if (not rotation)
		return;

	m_LocalTransform.rotate(rotation);

	setWorldTransformDirty();
}

void fro::GameObject::localScale(glm::vec2 const& scale)
{
	if (not scale.x and not scale.y)
		return;

	m_LocalTransform.scale(scale);

	setWorldTransformDirty();
}

void fro::GameObject::worldTransform(TransformationMatrix2D const& transformation)
{
	if (transformation.getTransformation() == glm::mat3x3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f))
		return;

	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.transform(transformation.getTransformation());

	calculateLocalTransform();
}

void fro::GameObject::worldTranslate(glm::vec2 const& translation)
{
	if (not translation.x and not translation.y)
		return;

	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.translate(translation);

	calculateLocalTransform();
}

void fro::GameObject::worldRotate(float const rotation)
{
	if (not rotation)
		return;

	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.rotate(rotation);

	calculateLocalTransform();
}

void fro::GameObject::worldScale(glm::vec2 const& scale)
{
	if (not scale.x and not scale.y)
		return;

	if (m_IsWorldTransformDirty)
	{
		calculateWorldTransform();
		m_IsWorldTransformDirty = false;
	}

	m_WorldTransform.scale(scale);

	calculateLocalTransform();
}

void fro::GameObject::setLocalTransformation(TransformationMatrix2D const& transformation)
{
	if (getLocalTransform().getTransformation() == transformation.getTransformation())
		return;

	m_LocalTransform = transformation;

	setWorldTransformDirty();
}

void fro::GameObject::setLocalTranslation(glm::vec2 const& translation)
{
	if (getLocalTransform().getTranslation() == translation)
		return;

	m_LocalTransform.setTranslation(translation);

	setWorldTransformDirty();
}

void fro::GameObject::setLocalRotation(float const rotation)
{
	if (getLocalTransform().getRotation() == rotation)
		return;

	m_LocalTransform.setRotation(rotation);

	setWorldTransformDirty();
}

void fro::GameObject::setLocalScale(glm::vec2 const& scale)
{
	if (getLocalTransform().getScale() == scale)
		return;

	m_LocalTransform.setScale(scale);

	setWorldTransformDirty();
}

void fro::GameObject::setWorldTransformation(TransformationMatrix2D const& transformation)
{
	if (getWorldTransform().getTransformation() == transformation.getTransformation())
		return;

	m_WorldTransform = transformation;

	calculateLocalTransform();

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
}

void fro::GameObject::setWorldTranslation(glm::vec2 const& translation)
{
	if (getWorldTransform().getTranslation() == translation)
		return;

	m_WorldTransform.setTranslation(translation);

	calculateLocalTransform();

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
}

void fro::GameObject::setWorldRotation(float const rotation)
{
	if (getWorldTransform().getRotation() == rotation)
		return;

	m_WorldTransform.setRotation(rotation);

	calculateLocalTransform();

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
}

void fro::GameObject::setWorldScale(glm::vec2 const& scale)
{
	if (getWorldTransform().getScale() == scale)
		return;

	m_WorldTransform.setScale(scale);

	calculateLocalTransform();

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
}

std::string_view fro::GameObject::getName() const
{
	return m_Name;
}

std::string_view fro::GameObject::getTag() const
{
	return m_Tag;
}

bool fro::GameObject::owns(Reference<GameObject> const gameObject) const
{
	return std::any_of(m_vChildren.begin(), m_vChildren.end(),
		[gameObject](Reference<GameObject> const child)
		{
			return gameObject == child;
		});
}

fro::Reference<fro::GameObject> fro::GameObject::getParent() const
{
	return m_Parent;
}

std::vector<fro::Reference<fro::GameObject>> const& fro::GameObject::getChildren() const
{
	return m_vChildren;
}

fro::Reference<fro::GameObject> fro::GameObject::getChild(std::string_view const name) const
{
	auto const iFoundChild
	{
		std::find_if(m_vChildren.begin(), m_vChildren.end(),
			[name](Reference<GameObject> const child)
			{
				return name == child.get().getName();
			})
	};

	if (iFoundChild == m_vChildren.end())
		return {};

	return *iFoundChild;
}

fro::TransformationMatrix2D const& fro::GameObject::getLocalTransform() const
{
	return m_LocalTransform;
}

fro::TransformationMatrix2D const& fro::GameObject::getWorldTransform() const
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
void fro::GameObject::setWorldTransformDirty()
{
	if (m_IsWorldTransformDirty)
		return;

	m_IsWorldTransformDirty = true;

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
}

void fro::GameObject::calculateLocalTransform()
{
	if (m_Parent.valid())
		m_LocalTransform = getWorldTransform() / m_Parent.get().getWorldTransform();
	else
		m_LocalTransform = getWorldTransform();
}

void fro::GameObject::calculateWorldTransform() const
{
	if (m_Parent.valid())
		m_WorldTransform = getLocalTransform() * m_Parent.get().getWorldTransform();
	else
		m_WorldTransform = getLocalTransform();
}
#pragma endregion PrivateMethods