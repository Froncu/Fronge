#if not defined fro_SCENE_MANAGER_H
#define fro_SCENE_MANAGER_H

#include "Reference.hpp"
#include "Scene.h"
#include "Singleton.hpp"

#include <vector>

struct SDL_Renderer;

namespace fro
{
	class SceneManager final : public Singleton<SceneManager>
	{
		friend class Singleton<SceneManager>;

	public:
		virtual ~SceneManager() override = default;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void lateUpdate(float const deltaSeconds) const;

		void render() const;
		void display() const;

		fro::Reference<fro::Scene> loadScene(std::string name);
		bool unloadScene(std::string name);

		fro::Reference<fro::Scene> getScene(std::string_view const name);

	private:
		SceneManager() = default;
		SceneManager(SceneManager const&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;

		SceneManager& operator=(SceneManager const&) = delete;
		SceneManager& operator=(SceneManager&&) noexcept = delete;

		std::vector<Scene>::iterator findScene(std::string_view const name);

		std::vector<Scene> m_vScenes{};
		std::size_t m_ActiveSceneIndex{};
	};
}

#endif