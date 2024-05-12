#if not defined fro_SCENE_H
#define fro_SCENE_H

#include "GameObject.h"

#include <memory>
#include <vector>

namespace fro
{
	class Scene final
	{
	public:
		Scene() = default;

		~Scene() = default;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void render() const;
		void display() const;
		GameObject& addGameObject();

		bool isActive() const;

	private:
		Scene(Scene const&) = delete;
		Scene(Scene&&) noexcept = delete;

		Scene& operator=(Scene const&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		bool m_IsActive{ true };
		std::vector<std::unique_ptr<GameObject>> m_vpGameObjects{};
	};
}

#endif