#include "GameObject.h"

#pragma region PrivateMethods
void fro::GameObject::update() const
{
	for (const auto& pair : m_mpBehaviours)
		pair.second->update();
}

void fro::GameObject::render(SDL_Renderer* const pRenderer) const
{
	for (const auto& pair : m_mpRenderables)
		pair.second->render(pRenderer);
}
#pragma endregion PrivateMethods