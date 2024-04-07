#include "GameObject.h"

#pragma region PublicMethods
void fro::GameObject::setParent(GameObject* const pParent, bool keepWorldPosition)
{
	if (pParent == this || pParent == m_pParent || owns(pParent))
		return;

	if (m_pParent)
		m_pParent->m_spChildren.erase(this);

	Transform& transform{ *getComponent<Transform>() };
	const glm::vec2* pOldWorldPosition{};
	if (keepWorldPosition)
		pOldWorldPosition = &transform.getWorldPosition();

	m_pParent = pParent;

	if (m_pParent)
		m_pParent->m_spChildren.insert(this);

	if (pOldWorldPosition)
		transform.setLocalPosition(*pOldWorldPosition);
}

const fro::GameObject* const fro::GameObject::getParent() const
{
	return m_pParent;
}

const std::set<const fro::GameObject*>& fro::GameObject::getChildren() const
{
	return m_spChildren;
}

bool fro::GameObject::owns(const GameObject* const pGameObject) const
{
	for (const GameObject* const pChild : m_spChildren)
		if (pGameObject == pChild)
			return true;

	return false;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::GameObject::update() const
{
	for (const auto& pair : m_mpBehaviours)
		pair.second->update();
}

void fro::GameObject::render() const
{
	for (const auto& pair : m_mpRenderables)
		pair.second->render();
}
#pragma endregion PrivateMethods