#include "IdleState.h"

#include "AttackState.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "InputManager.h"
#include "MoveState.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::IdleState::IdleState(Reference<GameObject> const parentingGameObject)
	: State(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::IdleState::update(float const)
{
	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.getActionStrength("attack"))
		return m_ParentingGameObject.get().forceGetComponent<AttackState>();

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };
	if (inputAxis2D.x or inputAxis2D.y)
		return m_ParentingGameObject.get().forceGetComponent<MoveState>();

	return {};
}

void fro::IdleState::enter(Reference<State> const)
{
	Reference<SpriteAnimator> spriteAnimator{ m_ParentingGameObject.get().getComponent<SpriteAnimator>()};
	spriteAnimator.get().setActiveAnimation("walking");
	spriteAnimator.get().pause();

	m_ParentingGameObject.get().getComponent<GridMovement>().get().setMoveDirection({0.0f, 0.0f});
}
#pragma endregion PublicMethods