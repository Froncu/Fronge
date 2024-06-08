#if not defined fro_SCENE_H
#define fro_SCENE_H

#include "BaseReferencable.h"
#include "Defines.hpp"
#include "GameObject.h"
#include "EventQueue.hpp"

#include <string>
#include <vector>

namespace fro
{
	class Scene final : public BaseReferencable
	{
	public:
		Scene(std::string name);
		Scene(Scene const&) = delete;
		Scene(Scene&&) noexcept = default;

		~Scene() = default;

		Scene& operator=(Scene const&) = delete;
		Scene& operator=(Scene&&) noexcept = default;

		void awake();

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void lateUpdate(float const deltaSeconds) const;

		void render() const;
		void display() const;

		void cleanUp();

		std::string_view getName() const;

		Reference<GameObject> addGameObject(std::string name);
		fro_NODISCARD Reference<GameObject> getGameObject(std::string_view const name);
		fro_NODISCARD Reference<GameObject> forceGetGameObject(std::string const& name);
		void removeGameObject(std::string_view const name);

	private:
		using GameObjects = std::vector<GameObject>;

		GameObjects::iterator findGameObject(std::string_view const name);

		EventQueue<std::string_view> m_GameObjectsToRemove
		{
			[this](std::string_view const gameObjectName)
			{
				auto const iFoundGameObject{ findGameObject(gameObjectName) };

				if (iFoundGameObject == m_GameObjects.end())
					return;

				m_GameObjects.erase(iFoundGameObject);
			}
		};

		std::string m_Name;

		GameObjects m_GameObjects{};
		std::vector<Reference<GameObject>> m_vGameObjectsToAwake{};
	};
}

#endif