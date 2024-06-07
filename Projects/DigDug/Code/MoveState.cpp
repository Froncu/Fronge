#include "MoveState.h"

#include "AttackState.h"
#include "IdleState.h"
#include "InputManager.h"

#pragma region Constructors/Destructor
fro::MoveState::MoveState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::MoveState::update(float const)
{
	InputManager& inputManager{ InputManager::getInstance() };

	if (inputManager.isActionJustPressed("attack"))
		return parentingGameObject.get().forceGetComponent<AttackState>();

	glm::vec2 const& inputAxis2D{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };

	if (not inputAxis2D.x and not inputAxis2D.y)
		return parentingGameObject.get().forceGetComponent<IdleState>();

	m_GridMovement.get().setMoveDirection(inputAxis2D);
	return {};
}

void fro::MoveState::enter(Reference<State> const)
{
	m_Pump.get().setActive(false);

	m_AudioService.playMusic("Sounds/In-Game Music.mp3");

	m_SpriteAnimator.get().setActiveAnimation("walking");
	m_SpriteAnimator.get().play();
}

void fro::MoveState::exit(Reference<State> const)
{
	m_AudioService.pauseMusic();

	parentingGameObject.get().getComponent<GridMovement>().get().setMoveDirection({});
}
#pragma endregion PublicMethods