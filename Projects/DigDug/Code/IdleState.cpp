#include "IdleState.h"

#include "AttackState.h"
#include "InputManager.h"
#include "MoveState.h"

#pragma region Constructors/Destructor
fro::IdleState::IdleState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::IdleState::update(float const)
{
	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.isActionJustPressed("attack"))
		return parentingGameObject.get().forceGetComponent<AttackState>();

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };
	if (inputAxis2D.x or inputAxis2D.y)
		return parentingGameObject.get().forceGetComponent<MoveState>();

	return {};
}

void fro::IdleState::enter(Reference<State> const)
{
 	m_Pump.get().setActive(false);

	m_SpriteAnimator.get().setActiveAnimation("walking");
	m_SpriteAnimator.get().pause();

	m_GridMovement.get().setMoveDirection({ 0.0f, 0.0f });
}
#pragma endregion PublicMethods