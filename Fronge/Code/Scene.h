#pragma once

#include "GameObject.h"

#include <string>
#include <vector>
#include <memory>

struct SDL_Renderer;

namespace fro
{
	class Scene final
	{
		friend class SceneManager;

	public:
		~Scene() = default;

		GameObject& addGameObject();

	private:
		Scene(const std::string& name);
		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;

		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		void update() const;
		void render() const;
		void display() const;

		bool m_IsActive{ true };
		std::vector<std::unique_ptr<GameObject>> m_vpGameObjects{};
	};
}