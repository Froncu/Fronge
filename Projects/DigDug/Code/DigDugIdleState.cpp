#include "DigDugIdleState.h"

#include "DigDugAttackState.h"
#include "DigDugDeadState.h"
#include "InputManager.h"
#include "DigDugMoveState.h"

#pragma region Constructors/Destructor
fro::DigDugIdleState::DigDugIdleState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::DigDugIdleState::awake()
{
	m_RigidBody = parentingGameObject.get().forceGetComponent<RigidBody>();

	m_SpriteAnimator = parentingGameObject.get().forceGetComponent<SpriteAnimator>();
	m_GridMovement = parentingGameObject.get().forceGetComponent<GridMovement>();

	m_Pump = parentingGameObject.get().getParentingScene().get().forceGetGameObject("pump");
}

fro::Reference<fro::State> fro::DigDugIdleState::update(float const)
{
	if (m_RigidBody.valid() and m_RigidBody.get().isOverlapping("enemy"))
		return parentingGameObject.get().forceGetComponent<DigDugDeadState>();

	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.isActionJustPressed("attackDigDug"))
		return parentingGameObject.get().forceGetComponent<DigDugAttackState>();

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRightDigDug", "moveLeftDigDug", "moveUpDigDug", "moveDownDigDug") };
	if (inputAxis2D.x or inputAxis2D.y)
		return parentingGameObject.get().forceGetComponent<DigDugMoveState>();

	return {};
}

void fro::DigDugIdleState::enter(Reference<State> const)
{
	if (m_Pump.valid())
		m_Pump.get().setActive(false);

	if (m_SpriteAnimator.valid())
	{
		m_SpriteAnimator.get().setActiveAnimation("walking");
		m_SpriteAnimator.get().pause();
	}

	if (m_GridMovement.valid())
		m_GridMovement.get().setMoveDirection({ 0.0f, 0.0f });
}
#pragma endregion PublicMethods