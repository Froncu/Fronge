#include "MoveCommand.h"

#include "GameObject.h"
#include "Timer.h"

#pragma region Constructors/Destructor
fro_GENERATED_GAME_OBJECT_COMMAND_CONSTRUCTOR(MoveCommand)
{
}

fro_GENERATED_GAME_OBJECT_COMMAND_DESTRUCTOR(MoveCommand)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::MoveCommand::setMoveDirection(const glm::vec2& moveDirection)
{
	m_MoveDirection = glm::normalize(moveDirection);
}

void fro::MoveCommand::setMoveSpeed(float moveSpeed)
{
	m_MoveSpeed = moveSpeed;
}
#pragma endregion PublicMethods



#pragma region Operators
fro_GENERATED_GAME_OBJECT_COMMAND_OPERATOR(MoveCommand)
{
	getCommandedGameObject().getComponent<Transform>()->localTranslate(Timer::getDeltaSeconds() * m_MoveSpeed * m_MoveDirection);
}
#pragma endregion Operators