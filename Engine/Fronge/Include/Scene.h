#if not defined fro_SCENE_H
#define fro_SCENE_H

#include "GameObject.h"
#include "References.hpp"

#include <vector>

namespace fro
{
	class Scene final : public Referencable<Scene>
	{
	public:
		Scene() = default;
		Scene(Scene const&) = default;
		Scene(Scene&&) noexcept = default;

		~Scene() = default;

		Scene& operator=(Scene const&) = default;
		Scene& operator=(Scene&&) noexcept = default;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void render() const;
		void display() const;
		fro::Reference<fro::GameObject> addGameObject();

		bool isActive() const;

	private:
		bool m_IsActive{ true };
		std::vector<GameObject> m_vGameObjects{};
	};
}

#endif