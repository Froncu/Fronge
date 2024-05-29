#include "GameObject.h"

#include <algorithm>

#pragma region Constructors/Destructor
fro::GameObject::GameObject()
{
	m_mpComponents.emplace(typeid(Transform), std::make_unique<Transform>(*this));
}

fro::GameObject::GameObject(GameObject&& other) noexcept
	: m_mpComponents{ std::move(other.m_mpComponents) }

	, m_vpFixedBehaviours{ std::move(other.m_vpFixedBehaviours) }
	, m_vpBehaviours{ std::move(other.m_vpBehaviours) }
	, m_vpRenderables{ std::move(other.m_vpRenderables) }
	, m_vpGUIs{ std::move(other.m_vpGUIs) }

	, m_spChildren{ std::move(other.m_spChildren) }
	, m_pParent{ other.m_pParent }

	, m_IsActive{ other.m_IsActive }
{
	other.m_pParent = nullptr;
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::GameObject& fro::GameObject::operator=(GameObject&& other) noexcept
{
	m_mpComponents = std::move(other.m_mpComponents);

	m_vpFixedBehaviours = std::move(other.m_vpFixedBehaviours);
	m_vpBehaviours = std::move(other.m_vpBehaviours);
	m_vpRenderables = std::move(other.m_vpRenderables);
	m_vpGUIs = std::move(other.m_vpGUIs);

	m_spChildren = std::move(other.m_spChildren);
	m_pParent = other.m_pParent;
	other.m_pParent = nullptr;

	m_IsActive = other.m_IsActive;

	return *this;
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::GameObject::fixedUpdate(float const fixedDeltaSeconds) const
{
	if (m_IsActive)
		for (FixedBehaviour* const pFixedBehaviour : m_vpFixedBehaviours)
			pFixedBehaviour->fixedUpdate(fixedDeltaSeconds);
}

void fro::GameObject::update(float const deltaSeconds) const
{
	if (m_IsActive)
		for (Behaviour* const pBehaviour : m_vpBehaviours)
			pBehaviour->update(deltaSeconds);
}

void fro::GameObject::render() const
{
	if (m_IsActive)
		for (Renderable* const pRenderable : m_vpRenderables)
			pRenderable->render();
}

void fro::GameObject::display() const
{
	if (m_IsActive)
		for (GUI* const pGUI : m_vpGUIs)
			pGUI->display();
}

void fro::GameObject::setActive(bool const isActive)
{
	m_IsActive = isActive;
}

void fro::GameObject::setParent(GameObject* const pParent, bool const keepWorldTransform)
{
	if (pParent == this || pParent == m_pParent || owns(pParent))
		return;

	if (m_pParent)
		m_pParent->m_spChildren.erase(this);

	Transform& transform{ *getComponent<Transform>() };
	TransformationMatrix2D oldWorldTransform;
	if (keepWorldTransform)
		oldWorldTransform = transform.getWorldTransform();

	m_pParent = pParent;

	if (m_pParent)
		m_pParent->m_spChildren.insert(this);

	if (m_pParent && keepWorldTransform)
		transform.setWorldTransformation(oldWorldTransform);
	else
		transform.setWorldTransformDirty();
}

bool fro::GameObject::owns(GameObject const* const pGameObject) const
{
	return std::any_of(m_spChildren.begin(), m_spChildren.end(),
		[pGameObject](GameObject const* const pChild)
		{
			return pGameObject == pChild;
		});
}

fro::GameObject* fro::GameObject::getParent() const
{
	return m_pParent;
}

std::set<fro::GameObject*> const& fro::GameObject::getChildren() const
{
	return m_spChildren;
}
#pragma endregion PublicMethods