#if not defined fro_SCENE_MANAGER_H
#define fro_SCENE_MANAGER_H

#include "Singleton.hpp"
#include "Scene.h"

#include <vector>
#include <memory>

struct SDL_Renderer;

namespace fro
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager() = default;

		virtual ~SceneManager() override = default;

		void update() const;
		void render() const;
		void display() const;
		Scene& addScene();

	private:
		SceneManager(SceneManager const&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;

		SceneManager& operator=(SceneManager const&) = delete;
		SceneManager& operator=(SceneManager&&) noexcept = delete;

		std::vector<std::unique_ptr<Scene>> m_vpScenes{};
	};
}

#endif