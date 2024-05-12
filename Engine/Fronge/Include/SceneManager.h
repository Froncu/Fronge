#if not defined fro_SCENE_MANAGER_H
#define fro_SCENE_MANAGER_H

#include "Scene.h"
#include "Singleton.hpp"

#include <memory>
#include <vector>

struct SDL_Renderer;

namespace fro
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager() = default;

		virtual ~SceneManager() override = default;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
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