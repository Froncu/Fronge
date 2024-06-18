#include "GameObject.h"

#include "Scene.h"

#include <algorithm>

#pragma region Constructors/Destructor
fro::GameObject::GameObject(Reference<Scene> const parentingScene, std::string name)
	: m_ParentingScene{ parentingScene }
	, m_Name{ std::move(name) }
{
}

fro::GameObject::GameObject(GameObject&& other) noexcept
	: BaseReferencable(std::move(other))

	, activeStateChanged{ std::move(other.activeStateChanged) }

	, m_ParentingScene{ std::move(other.m_ParentingScene) }
	, m_Name{ std::move(other.m_Name) }

	, m_Tag{ std::move(other.m_Tag) }

	, m_Parent{ std::move(other.m_Parent) }
	, m_vChildren{ std::move(other.m_vChildren) }

	, m_LocalTransform{ std::move(other.m_LocalTransform) }
	, m_WorldTransform{ std::move(other.m_WorldTransform) }

	, m_mpComponents{ std::move(other.m_mpComponents) }
	, m_vBehaviours{ std::move(other.m_vBehaviours) }
	, m_vRenderables{ std::move(other.m_vRenderables) }
	, m_vGUIs{ std::move(other.m_vGUIs) }

	, m_vComponentsToAwake{ std::move(other.m_vComponentsToAwake) }

	, m_IsActive{ other.m_IsActive }
	, m_IsWorldTransformDirty{ other.m_IsWorldTransformDirty }

{
	other.m_IsWorldTransformDirty = false;
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::GameObject& fro::GameObject::operator=(GameObject&& other) noexcept
{
	BaseReferencable::operator=(std::move(other));

	activeStateChanged = std::move(other.activeStateChanged);

	m_ParentingScene = std::move(other.m_ParentingScene);
	m_Name = std::move(other.m_Name);

	m_Tag = std::move(other.m_Tag);

	m_Parent = std::move(other.m_Parent);
	m_vChildren = std::move(other.m_vChildren);

	m_LocalTransform = std::move(other.m_LocalTransform);
	m_WorldTransform = std::move(other.m_WorldTransform);

	m_mpComponents = std::move(other.m_mpComponents);
	m_vBehaviours = std::move(other.m_vBehaviours);
	m_vRenderables = std::move(other.m_vRenderables);
	m_vGUIs = std::move(other.m_vGUIs);

	m_vComponentsToAwake = std::move(other.m_vComponentsToAwake);

	m_IsActive = other.m_IsActive;
	m_IsWorldTransformDirty = other.m_IsWorldTransformDirty;

	return *this;
}

void fro::GameObject::awake()
{
	while (m_vComponentsToAwake.size())
	{
		if (m_vComponentsToAwake.front().valid())
			m_vComponentsToAwake.front().get().awake();

		m_vComponentsToAwake.erase(m_vComponentsToAwake.begin());
	}
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::GameObject::fixedUpdate(float const fixedDeltaSeconds) const
{
	if (isActive())
		for (Reference<Behaviour> const behaviour : m_vBehaviours)
			behaviour.get().fixedUpdate(fixedDeltaSeconds);
}

void fro::GameObject::update(float const deltaSeconds) const
{
	if (isActive())
		for (Reference<Behaviour> const behaviour : m_vBehaviours)
			behaviour.get().update(deltaSeconds);
}

void fro::GameObject::lateUpdate(float const deltaSeconds) const
{
	if (isActive())
		for (Reference<Behaviour> const behaviour : m_vBehaviours)
			behaviour.get().lateUpdate(deltaSeconds);
}

void fro::GameObject::render() const
{
	if (isActive())
		for (Reference<Renderable> const renderable : m_vRenderables)
			renderable.get().render();
}

void fro::GameObject::display() const
{
	if (isActive())
		for (Reference<GUI> const GUI : m_vGUIs)
			GUI.get().display();
}

void fro::GameObject::destroy()
{
	m_ParentingScene.get().removeGameObject(m_Name);
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

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
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

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
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

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
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

	for (Reference<GameObject> const child : m_vChildren)
		child.get().setWorldTransformDirty();
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

bool fro::GameObject::setParent(Reference<GameObject> const parent, bool const keepWorldTransform)
{
	if (parent == this or parent == m_Parent or getGameObject(parent.get().getName()).valid())
		return false;

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

	return true;
}

void fro::GameObject::setTag(std::string tag)
{
	m_Tag = std::move(tag);
}

void fro::GameObject::setActive(bool const isActive)
{
	if (m_IsActive == isActive)
		return;

	m_IsActive = isActive;

	if (m_Parent.valid() and not m_Parent.get().isActive())
		return;

	notifyActiveStateChanged(isActive);
}

fro::Reference<fro::GameObject> fro::GameObject::getParent() const
{
	return m_Parent;
}

fro::Reference<fro::GameObject> fro::GameObject::getGameObject(std::string_view const name)
{
	if (m_Name == name)
		return this;

	Reference<GameObject> foundGameObject{};
	for (Reference<GameObject> const child : m_vChildren)
	{
		foundGameObject = child.get().getGameObject(name);
		if (foundGameObject.valid())
			return foundGameObject;
	}

	return foundGameObject;
}

fro::Reference<fro::GameObject> fro::GameObject::forceGetGameObject(std::string const& name)
{
	Reference<GameObject> gameObject{ getGameObject(name) };
	if (gameObject.valid())
		return gameObject;

	gameObject = m_ParentingScene.get().addGameObject(name);
	if (gameObject.valid())
		gameObject.get().setParent(this, false);

	return gameObject;
}

std::vector<fro::Reference<fro::GameObject>> const& fro::GameObject::getChildren() const
{
	return m_vChildren;
}

fro::Reference<fro::Scene> fro::GameObject::getParentingScene() const
{
	return m_ParentingScene;
}

std::string_view fro::GameObject::getName() const
{
	return m_Name;
}

std::string_view fro::GameObject::getTag() const
{
	return m_Tag;
}

bool fro::GameObject::isActive() const
{
	if (m_Parent.valid() and not m_Parent.get().isActive())
		return false;

	return m_IsActive;
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

void fro::GameObject::notifyActiveStateChanged(bool const newIsActive) const
{
	activeStateChanged.notifySubscribers(newIsActive);

	for (Reference<GameObject> const child : m_vChildren)
		if (newIsActive and child.get().m_IsActive)
			child.get().notifyActiveStateChanged(true);
		else if (not newIsActive)
			child.get().notifyActiveStateChanged(false);
}
#pragma endregion PrivateMethods