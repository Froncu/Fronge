#include "IdleState.h"

#include "AttackState.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "InputManager.h"
#include "MoveState.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::IdleState::IdleState(GameObject& parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
std::unique_ptr<fro::State> fro::IdleState::update(float const)
{
	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.getActionStrength("attack"))
		return std::make_unique<AttackState>(getParentingGameObject());

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };
	if (inputAxis2D.x or inputAxis2D.y)
		return std::make_unique<MoveState>(getParentingGameObject());

	return nullptr;
}

void fro::IdleState::enter(std::unique_ptr<State> const&)
{
	SpriteAnimator& spriteAnimator{ *getParentingGameObject().getComponent<SpriteAnimator>()};
	spriteAnimator.setActiveAnimation("walking");
	spriteAnimator.pause();

	getParentingGameObject().getComponent<GridMovement>()->setMoveDirection({0.0f, 0.0f});
}
#pragma endregion PublicMethods