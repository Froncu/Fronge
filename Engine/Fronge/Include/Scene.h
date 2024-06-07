#if not defined fro_SCENE_H
#define fro_SCENE_H

#include "GameObject.h"
#include "BaseReferencable.h"
#include "Defines.hpp"

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

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void lateUpdate(float const deltaSeconds) const;

		void render() const;
		void display() const;

		std::string_view getName() const;

		Reference<GameObject> addGameObject(std::string name);
		fro_NODISCARD Reference<GameObject> getGameObject(std::string_view const name);
		fro_NODISCARD Reference<GameObject> forceGetGameObject(std::string const& name);
		bool removeGameObject(std::string const& name);

	private:
		using GameObjects = std::vector<GameObject>;

		GameObjects::iterator findGameObject(std::string_view const name);

		std::string m_Name;

		GameObjects m_GameObjects{};
	};
}

#endif