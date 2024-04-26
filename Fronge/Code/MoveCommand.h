#pragma once

#include "GameObjectCommand.h"

#include <vec2.hpp>

namespace fro
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject const& commandedGameObject);

		virtual ~MoveCommand() override = default;

		virtual void operator()() override;

		void setMoveDirection(glm::vec2 const& moveDirection);
		void setMoveSpeed(float const moveSpeed);

	private:
		MoveCommand(MoveCommand const&) = delete;
		MoveCommand(MoveCommand&&) noexcept = delete;

		MoveCommand& operator=(MoveCommand const&) = delete;
		MoveCommand& operator=(MoveCommand&&) noexcept = delete;

		glm::vec2 m_MoveDirection{};
		float m_MoveSpeed{ 100.0f };
	};
}