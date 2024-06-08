#include "DigDugMoveState.h"

#include "DigDugAttackState.h"
#include "DigDugDeadState.h"
#include "DigDugIdleState.h"
#include "InputManager.h"

#pragma region Constructors/Destructor
fro::DigDugMoveState::DigDugMoveState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::DigDugMoveState::awake()
{
	m_RigidBody = parentingGameObject.get().forceGetComponent<RigidBody>();

	m_SpriteAnimator = parentingGameObject.get().forceGetComponent<SpriteAnimator>();

	m_GridMovement = parentingGameObject.get().forceGetComponent<GridMovement>();

	m_Pump = parentingGameObject.get().getParentingScene().get().forceGetGameObject("pump");
}

fro::Reference<fro::State> fro::DigDugMoveState::update(float const)
{
	if (m_RigidBody.valid() and m_RigidBody.get().isOverlapping("enemy"))
		return parentingGameObject.get().forceGetComponent<DigDugDeadState>();

	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.isActionJustPressed("attackDigDug"))
		return parentingGameObject.get().forceGetComponent<DigDugAttackState>();

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRightDigDug", "moveLeftDigDug", "moveUpDigDug", "moveDownDigDug") };

	if (not inputAxis2D.x and not inputAxis2D.y)
		return parentingGameObject.get().forceGetComponent<DigDugIdleState>();

	if (m_GridMovement.valid())
		m_GridMovement.get().setMoveDirection(inputAxis2D);

	return {};
}

void fro::DigDugMoveState::enter(Reference<State> const)
{
	if (m_Pump.valid())
		m_Pump.get().setActive(false);

	m_AudioService.playMusic("Sounds/In-Game Music.mp3");

	if (m_SpriteAnimator.valid())
	{
		m_SpriteAnimator.get().setActiveAnimation("walking");
		m_SpriteAnimator.get().play();
	}
}

void fro::DigDugMoveState::exit(Reference<State> const)
{
	m_AudioService.pauseMusic();

	parentingGameObject.get().getComponent<GridMovement>().get().setMoveDirection({});
}
#pragma endregion PublicMethods