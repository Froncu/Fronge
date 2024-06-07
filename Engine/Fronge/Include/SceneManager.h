#if not defined fro_SCENE_MANAGER_H
#define fro_SCENE_MANAGER_H

#include "Defines.hpp"
#include "Reference.hpp"
#include "Scene.h"
#include "Singleton.hpp"

#include <vector>

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

		bool setActiveScene(Reference<Scene> const scene);

		Reference<Scene> addScene(std::string name);
		fro_NODISCARD Reference<Scene> getScene(std::string_view const name);
		fro_NODISCARD Reference<Scene> forceGetScene(std::string const& name);
		bool removeScene(std::string const& name);

	private:
		using Scenes = std::vector<Scene>;

		SceneManager() = default;
		SceneManager(SceneManager const&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;

		SceneManager& operator=(SceneManager const&) = delete;
		SceneManager& operator=(SceneManager&&) noexcept = delete;

		Scenes::iterator findScene(std::string_view const name);

		Scenes m_Scenes{};
		Reference<Scene> m_ActiveScene{};
	};
}

#endif