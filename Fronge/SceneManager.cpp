#include "SceneManager.h"

#include "Scene.h"

#pragma region StaticDataMembers
std::vector<std::unique_ptr<fro::Scene>> fro::SceneManager::m_vpSCENES{};
#pragma endregion StaticDataMembers



#pragma region PublicMethods
fro::Scene& fro::SceneManager::addScene(const std::string& name)
{
	return *m_vpSCENES.emplace_back(decltype(m_vpSCENES)::value_type{ new Scene(name) });
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::SceneManager::update()
{
	for (const auto& pScene : m_vpSCENES)
		if (pScene->m_IsActive)
			pScene->update();
}

void fro::SceneManager::render(SDL_Renderer* const pRenderer)
{
	for (const auto& pScene : m_vpSCENES)
		if (pScene->m_IsActive)
			pScene->render(pRenderer);
}
#pragma endregion PrivateMethods