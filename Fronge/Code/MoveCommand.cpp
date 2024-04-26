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
	getCommandedGameObject().getComponent<Transform>()->localTranslate(Timer::getInstance().getDeltaSeconds() * m_MoveSpeed * m_MoveDirection);
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