#pragma once

#include "Fronge.h"

#include <string>
#include <vector>
#include <memory>

struct SDL_Renderer;

namespace fro
{
	class Scene;

	class SceneManager final
	{
		friend void Fronge::run();

	public:
		static Scene& addScene(const std::string& name);

	private:
		SceneManager() = delete;
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;

		~SceneManager() = delete;

		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager& operator=(SceneManager&&) noexcept = delete;

		static void update();
		static void render(SDL_Renderer* const pRenderer);

		static std::vector<std::unique_ptr<Scene>> m_vpSCENES;
	};
}