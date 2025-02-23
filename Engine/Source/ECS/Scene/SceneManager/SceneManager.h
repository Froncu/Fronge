#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "froch.hpp"

#include "ECS/Scene/Scene.hpp"
#include "Events/Systems/EventDispatcher.hpp"

namespace fro
{
	class SceneManager final
	{
	public:
		FRO_API static void shutDown();

		FRO_API static void doomAndAdd();
		
		FRO_API FRO_NODISCARD std::vector<Scene> const& getScenes() const;

		FRO_API FRO_NODISCARD static EventDispatcher<Reference<Scene> const, Reference<Scene> const>& getActiveSceneChangedEvent();
		FRO_API static bool setActiveScene(Scene const& scene);
		FRO_API FRO_NODISCARD static Reference<Scene> getActiveScene();

		FRO_API static std::optional<Scene> removeScene(Scene const& scene);

		template<typename... ArgumentTypes>
			requires std::constructible_from<Scene, ArgumentTypes...>
		static Scene& addScene(ArgumentTypes&&... arguments)
		{
			return sScenes.emplace_back(std::forward<ArgumentTypes>(arguments)...);
		}

	private:
		FRO_API static std::vector<Scene> sScenes;
		static Reference<Scene> sActiveScene;

		SceneManager() = delete;
		SceneManager(SceneManager const&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;

		~SceneManager() = delete;

		SceneManager& operator=(SceneManager const&) = delete;
		SceneManager& operator=(SceneManager&&) noexcept = delete;
	};
}

#endif