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
		Scene(Scene const&) = delete;
		Scene(Scene&&) noexcept = default;

		~Scene() = default;

		Scene& operator=(Scene const&) = delete;
		Scene& operator=(Scene&&) noexcept = default;
		bool operator==(Scene const& other) const;
		bool operator==(std::string_view const sceneName) const;
		std::strong_ordering operator<=>(Scene const& other) const;
		std::strong_ordering operator<=>(std::string_view const sceneName) const;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void lateUpdate(float const deltaSeconds) const;

		void render() const;
		void display() const;
		fro::Reference<fro::GameObject> addGameObject(std::string name, std::string tag = {});

	private:
		std::string m_Name;

		std::vector<GameObject> m_vGameObjects{};
	};
}

#endif