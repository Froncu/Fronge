#pragma once

#include "GameObject.h"

#include <vector>
#include <memory>

namespace fro
{
	class Scene final
	{
	public:
		Scene() = default;

		~Scene() = default;

		void update() const;
		void render() const;
		void display() const;
		GameObject& addGameObject();

		bool isActive() const;

	private:
		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;

		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		bool m_IsActive{ true };
		std::vector<std::unique_ptr<GameObject>> m_vpGameObjects{};
	};
}