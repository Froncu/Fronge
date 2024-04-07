#pragma once

#include "Singleton.hpp"

#include <string>
#include <vector>
#include <memory>

struct SDL_Renderer;

namespace fro
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
		fro_GENERATED_SINGLETON_BODY(SceneManager)

	public:
		Scene& addScene(const std::string& name);

		void update();
		void render();

	private:
		std::vector<std::unique_ptr<Scene>> m_vpScenes{};
	};
}