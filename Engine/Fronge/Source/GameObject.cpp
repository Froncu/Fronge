#include "GameObject.h"

#include <algorithm>

#pragma region Constructors/Destructor
fro::GameObject::GameObject()
{
	m_mpComponents.emplace(typeid(Transform), std::make_unique<Transform>(*this));
}

fro::GameObject::GameObject(GameObject&& other) noexcept
	: BaseReferencable(std::move(other))
	
	, m_mpComponents{ std::move(other.m_mpComponents) }

	, m_vFixedBehaviours{ std::move(other.m_vFixedBehaviours) }
	, m_vBehaviours{ std::move(other.m_vBehaviours) }
	, m_vRenderables{ std::move(other.m_vRenderables) }
	, m_vGUIs{ std::move(other.m_vGUIs) }

	, m_sChildren{ std::move(other.m_sChildren) }
	, m_Parent{ std::move(other.m_Parent) }

	, m_IsActive{ other.m_IsActive }
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::GameObject& fro::GameObject::operator=(GameObject&& other) noexcept
{
	BaseReferencable::operator=(std::move(other));

	m_mpComponents = std::move(other.m_mpComponents);

	m_vFixedBehaviours = std::move(other.m_vFixedBehaviours);
	m_vBehaviours = std::move(other.m_vBehaviours);
	m_vRenderables = std::move(other.m_vRenderables);
	m_vGUIs = std::move(other.m_vGUIs);

	m_sChildren = std::move(other.m_sChildren);
	m_Parent = std::move(other.m_Parent);

	m_IsActive = other.m_IsActive;

	return *this;
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::GameObject::fixedUpdate(float const fixedDeltaSeconds) const
{
	if (m_IsActive)
		for (Reference<FixedBehaviour> const fixedBehaviour : m_vFixedBehaviours)
			fixedBehaviour.get().fixedUpdate(fixedDeltaSeconds);
}

void fro::GameObject::update(float const deltaSeconds) const
{
	if (m_IsActive)
		for (Reference<Behaviour> const behaviour : m_vBehaviours)
			behaviour.get().update(deltaSeconds);
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
		m_Parent.get().m_sChildren.erase(this);

	Reference<Transform> transform{ getComponent<Transform>() };
	TransformationMatrix2D oldWorldTransform;
	if (keepWorldTransform)
		oldWorldTransform = transform.get().getWorldTransform();

	m_Parent = std::move(parent);

	if (m_Parent.valid())
		m_Parent.get().m_sChildren.insert(this);

	if (m_Parent.valid() && keepWorldTransform)
		transform.get().setWorldTransformation(oldWorldTransform);
	else
		transform.get().setWorldTransformDirty();
}

bool fro::GameObject::owns(Reference<GameObject> const gameObject) const
{
	return std::any_of(m_sChildren.begin(), m_sChildren.end(),
		[gameObject](Reference<GameObject> const child)
		{
			return gameObject == child;
		});
}

fro::Reference<fro::GameObject> fro::GameObject::getParent() const
{
	return m_Parent;
}

std::set<fro::Reference<fro::GameObject>> const& fro::GameObject::getChildren() const
{
	return m_sChildren;
}
#pragma endregion PublicMethods