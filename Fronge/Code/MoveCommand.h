#pragma once

#include "GameObjectCommand.h"

#include <vec2.hpp>

namespace fro
{
	class MoveCommand final : public GameObjectCommand
	{
		fro_GENERATED_GAME_OBJECT_COMMAND_BODY(MoveCommand)

	public:
		void setMoveDirection(const glm::vec2& moveDirection);
		void setMoveSpeed(float moveSpeed);

	private:
		glm::vec2 m_MoveDirection{};
		float m_MoveSpeed{ 100.0f };
	};
}