#include "FygarDefaultState.h"

#include "InputManager.h"
#include "FygarAttackState.h"

#pragma region Constructors/Destructor
fro::FygarDefaultState::FygarDefaultState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethod
fro::Reference<fro::State> fro::FygarDefaultState::fixedUpdate(float const)
{
	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.isActionJustPressed("attackFygar"))
		return parentingGameObject.get().forceGetComponent<FygarAttackState>();

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRightFygar", "moveLeftFygar", "moveUpFygar", "moveDownFygar") };
	m_GridMovement.get().setMoveDirection(inputAxis2D);

	return {};
}
#pragma endregion PublicMethod