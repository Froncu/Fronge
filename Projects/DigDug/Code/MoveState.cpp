#include "MoveState.h"

#include "AttackState.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "IdleState.h"
#include "InputManager.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::MoveState::MoveState(Reference<GameObject> const parentingGameObject)
	: State(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::MoveState::update(float const)
{
	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.getActionStrength("attack"))
		return m_ParentingGameObject.get().forceGetComponent<AttackState>();

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };

	if (not inputAxis2D.x and not inputAxis2D.y)
		return m_ParentingGameObject.get().forceGetComponent<IdleState>();

	m_ParentingGameObject.get().getComponent<GridMovement>().get().setMoveDirection(inputAxis2D);
	return {};
}

void fro::MoveState::enter(Reference<State> const)
{
	Reference<SpriteAnimator> spriteAnimator{ m_ParentingGameObject.get().getComponent<SpriteAnimator>() };
	spriteAnimator.get().setActiveAnimation("walking");
	spriteAnimator.get().play();
}

void fro::MoveState::exit(Reference<State> const)
{
	m_ParentingGameObject.get().getComponent<GridMovement>().get().setMoveDirection({});
}
#pragma endregion PublicMethods