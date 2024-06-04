#if not defined fro_SCENE_H
#define fro_SCENE_H

#include "GameObject.h"
#include "BaseReferencable.h"

#include <string>
#include <vector>

namespace fro
{
	class Scene final : public BaseReferencable
	{
	public:
		Scene(std::string name);
		Scene(Scene const&) = default;
		Scene(Scene&&) noexcept = default;

		~Scene() = default;

		Scene& operator=(Scene const&) = default;
		Scene& operator=(Scene&&) noexcept = default;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void lateUpdate(float const deltaSeconds) const;

		void render() const;
		void display() const;
		fro::Reference<fro::GameObject> addGameObject(std::string name);

	private:
		std::string const m_Name;

		std::vector<GameObject> m_vGameObjects{};
	};
}

#endif