#include "MoveCommand.h"

#include "GameObject.h"
#include "Timer.h"

#include <glm.hpp>

#pragma region Constructors/Destructor
fro::MoveCommand::MoveCommand(const GameObject& commandedGameObject) :
	GameObjectCommand(commandedGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
void fro::MoveCommand::operator()()
{
	getCommandedGameObject().getComponent<Transform>()->localTranslate(Timer::getInstance().getDeltaSeconds() * m_MoveSpeed * m_MoveDirection);
}
#pragma endregion Operators



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