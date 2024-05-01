#include "MoveCommand.h"

#include "GameObject.h"
#include "Timer.h"

#include <glm.hpp>

#pragma region Constructors/Destructor
fro::MoveCommand::MoveCommand(GameObject const& commandedGameObject)
	: GameObjectCommand(commandedGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
void fro::MoveCommand::operator()()
{
	glm::ivec2 const worldPosition{ getCommandedGameObject().getComponent<Transform>()->getWorldPosition() };

	glm::vec2 correctedMoveDirection{};
	if (m_MoveDirection.x)
	{
		if (int const offset{ worldPosition.y % 16 }; offset < 8)
			correctedMoveDirection.y = 1.0f;
		else if (offset > 8)
			correctedMoveDirection.y = -1.0f;
		else
			correctedMoveDirection = m_MoveDirection;
	}
	else
	{
		if (int const offset{ worldPosition.x % 16 }; offset > 8)
			correctedMoveDirection.x = -1.0f;
		else if (offset < 8)
			correctedMoveDirection.x = 1.0f;
		else
			correctedMoveDirection = m_MoveDirection;
	}

	getCommandedGameObject().getComponent<Transform>()->localTranslate(Timer::getInstance().getDeltaSeconds() * m_MoveSpeed * correctedMoveDirection);
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::MoveCommand::setMoveDirection(glm::vec2 const& moveDirection)
{
	m_MoveDirection = glm::normalize(moveDirection);
}

void fro::MoveCommand::setMoveSpeed(float const moveSpeed)
{
	m_MoveSpeed = moveSpeed;
}
#pragma endregion PublicMethods