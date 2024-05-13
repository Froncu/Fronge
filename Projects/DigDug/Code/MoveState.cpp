#include "MoveState.h"

#include "AttackState.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "IdleState.h"
#include "InputManager.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::MoveState::MoveState(GameObject const& parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
std::unique_ptr<fro::State> fro::MoveState::update(float const)
{
	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.getActionStrength("attack"))
		return std::make_unique<AttackState>(getParentingGameObject());

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };

	if (not inputAxis2D.x and not inputAxis2D.y)
		return std::make_unique<IdleState>(getParentingGameObject());

	getParentingGameObject().getComponent<GridMovement>()->setMoveDirection(inputAxis2D);
	return nullptr;
}

void fro::MoveState::enter(std::unique_ptr<State> const&)
{
	SpriteAnimator& spriteAnimator{ *getParentingGameObject().getComponent<SpriteAnimator>() };
	spriteAnimator.setActiveAnimation("walking");
	spriteAnimator.play();
}

void fro::MoveState::exit(std::unique_ptr<State> const&)
{
	getParentingGameObject().getComponent<GridMovement>()->setMoveDirection({});
}
#pragma endregion PublicMethods