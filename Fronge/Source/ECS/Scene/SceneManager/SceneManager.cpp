#include "froch.hpp"

#include "SceneManager.h"

namespace fro
{
	void SceneManager::shutDown()
	{
		sScenes.clear();
	}

	void SceneManager::doomAndAdd()
	{
		if (not sActiveScene.valid())
			return;

		sActiveScene->doomAndAdd();
	}

	std::vector<Scene> const& SceneManager::getScenes() const
	{
		return sScenes;
	}

	EventDispatcher<Reference<Scene> const, Reference<Scene> const>& SceneManager::getActiveSceneChangedEvent()
	{
		static EventDispatcher<Reference<Scene> const, Reference<Scene> const> event{};
		return event;
	}

	bool SceneManager::setActiveScene(Scene const& scene)
	{
		auto const foundScene
		{
			std::find_if(sScenes.begin(), sScenes.end(),
			[&scene](Scene const& storedScene)
			{
				return &scene == &storedScene;
			})
		};

		if (foundScene == sScenes.end() or (sActiveScene.valid() and &*sActiveScene == &scene))
			return false;

		Reference<Scene> oldActiveScene{ sActiveScene };
		sActiveScene = *foundScene;
		sActiveScene->doomAndAdd();
		getActiveSceneChangedEvent().notify(oldActiveScene, sActiveScene);
		return true;
	}

	Reference<Scene> SceneManager::getActiveScene()
	{
		return sActiveScene;
	}

	std::optional<Scene> SceneManager::removeScene(Scene const& scene)
	{
		auto const foundScene
		{
			std::find_if(sScenes.begin(), sScenes.end(),
			[&scene](Scene const& storedScene)
			{
				return &scene == &storedScene;
			})
		};

		if (foundScene == sScenes.end())
			return {};

		Scene removedScene{ std::move(*foundScene) };
		sScenes.erase(foundScene);

		if (sActiveScene.valid() and &*sActiveScene == &removedScene)
		{
			sActiveScene.reset();
			getActiveSceneChangedEvent().notify(removedScene, sActiveScene);
		};
		
		return removedScene;
	}

	std::vector<Scene> SceneManager::sScenes{};
	Reference<Scene> SceneManager::sActiveScene{};
}