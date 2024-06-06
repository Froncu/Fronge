#include "Scene.h"

#pragma region Constructors/Destructor
fro::Scene::Scene(std::string name)
	: m_Name{ std::move(name) }
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
bool fro::Scene::operator==(Scene const& other) const
{
	return m_Name == other.m_Name;
}

bool fro::Scene::operator==(std::string_view const sceneName) const
{
	return m_Name == sceneName;
}

std::strong_ordering fro::Scene::operator<=>(Scene const& other) const
{
	return m_Name <=> other.m_Name;
}

std::strong_ordering fro::Scene::operator<=>(std::string_view const sceneName) const
{
	return m_Name <=> sceneName;
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::Scene::fixedUpdate(float const fixedDeltaSeconds) const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.fixedUpdate(fixedDeltaSeconds);
}

void fro::Scene::update(float const deltaSeconds) const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.update(deltaSeconds);
}

void fro::Scene::lateUpdate(float const deltaSeconds) const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.lateUpdate(deltaSeconds);
}

void fro::Scene::render() const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.render();
}

void fro::Scene::display() const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.display();
}

fro::Reference<fro::GameObject> fro::Scene::addGameObject(std::string name, std::string tag)
{
	return m_vGameObjects.emplace_back(std::move(name), std::move(tag));
}
#pragma endregion PublicMethods