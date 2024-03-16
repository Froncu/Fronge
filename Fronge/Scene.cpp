#include "Scene.h"

#pragma region Constructors/Destructor
fro::Scene::Scene(const std::string&)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::GameObject& fro::Scene::addGameObject()
{
	return *m_vpGameObjects.emplace_back(decltype(m_vpGameObjects)::value_type(new GameObject()));
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Scene::update() const
{
	for (const auto& pGameObject : m_vpGameObjects)
		pGameObject->update();
}

void fro::Scene::render(SDL_Renderer* const pRenderer) const
{
	for (const auto& pGameObject : m_vpGameObjects)
		pGameObject->render(pRenderer);
}
#pragma endregion PrivateMethods