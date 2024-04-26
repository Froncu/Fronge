#pragma once

#include "GameObjectCommand.h"

#include <vec2.hpp>

namespace fro
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(const GameObject& commandedGameObject);

		virtual ~MoveCommand() override = default;

		virtual void operator()() override;

		void setMoveDirection(const glm::vec2& moveDirection);
		void setMoveSpeed(float moveSpeed);

	private:
		MoveCommand(const MoveCommand&) = delete;
		MoveCommand(MoveCommand&&) noexcept = delete;

		MoveCommand& operator=(const MoveCommand&) = delete;
		MoveCommand& operator=(MoveCommand&&) noexcept = delete;

		glm::vec2 m_MoveDirection{};
		float m_MoveSpeed{ 100.0f };
	};
}