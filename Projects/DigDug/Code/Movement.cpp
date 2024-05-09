#include "Movement.h"

#include "Console.hpp"
#include "GameObject.h"
#include "InputManager.h"
#include "Timer.h"

#include <glm.hpp>

#pragma region Constructors/Destructor
fro::Movement::Movement(GameObject const& parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Movement::update()
{
	glm::ivec2 const worldPosition{ getParentingGameObject().getComponent<Transform>()->getWorldTransform().getTranslation() };

	glm::vec2 const inputAxis
	{
		InputManager::getInstance().isActionJustPressed("moveRightDigDug"),
		0.0f
	};

	//if (inputAxis.x or inputAxis.y)
	//{
	//	glm::vec2 correctedMoveDirection{};
	//	if (inputAxis.x)
	//	{
	//		if (int const offset{ worldPosition.y % 16 }; offset < 8)
	//			correctedMoveDirection.y = 1.0f;
	//		else if (offset > 8)
	//			correctedMoveDirection.y = -1.0f;
	//		else
	//			correctedMoveDirection.x = inputAxis.x;
	//	}
	//	else
	//	{
	//		if (int const offset{ worldPosition.x % 16 }; offset > 8)
	//			correctedMoveDirection.x = -1.0f;
	//		else if (offset < 8)
	//			correctedMoveDirection.x = 1.0f;
	//		else
	//			correctedMoveDirection.y = -inputAxis.y;
	//	}

	//if (InputManager::getInstance().isActionJustReleased("moveRightDigDug"))
		Console::getInstance().log(InputManager::getInstance().getActionStrength("moveRightDigDug"));


		getParentingGameObject().getComponent<Transform>()->localTranslate(Timer::getInstance().getDeltaSeconds() * m_MoveSpeed * inputAxis);
	/*}*/
}

void fro::Movement::setMoveSpeed(float const moveSpeed)
{
	m_MoveSpeed = moveSpeed;
}
#pragma endregion PublicMethods