#include "GameObject.h"

#include <algorithm>

#pragma region PublicMethods
void fro::GameObject::fixedUpdate(float const fixedDeltaSeconds) const
{
	if (m_IsActive)
		for (auto const& pair : m_mpFixedBehaviours)
			pair.second->fixedUpdate(fixedDeltaSeconds);
}

void fro::GameObject::update(float const deltaSeconds) const
{
	if (m_IsActive)
		for (auto const& pair : m_mpBehaviours)
			pair.second->update(deltaSeconds);
}

void fro::GameObject::render() const
{
	if (m_IsActive)
		for (auto const& pair : m_mpRenderables)
			pair.second->render();
}

void fro::GameObject::display() const
{
	if (m_IsActive)
		for (auto const& pair : m_mpGUIs)
			pair.second->display();
}

void fro::GameObject::setActive(bool const isActive) const
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

fro::GameObject const* fro::GameObject::getParent() const
{
	return m_pParent;
}

std::set<fro::GameObject const*> const& fro::GameObject::getChildren() const
{
	return m_spChildren;
}
#pragma endregion PublicMethods