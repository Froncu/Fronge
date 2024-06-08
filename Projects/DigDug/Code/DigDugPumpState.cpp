#include "DigDugPumpState.h"

#include "GameObject.h"
#include "DigDugDeadState.h"
#include "DigDugIdleState.h"
#include "InputManager.h"
#include "DigDugMoveState.h"

#pragma region Constructors/Destructor
fro::DigDugPumpState::DigDugPumpState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::DigDugPumpState::awake()
{
	m_RigidBody = parentingGameObject.get().forceGetComponent<RigidBody>();

	m_SpriteAnimator = parentingGameObject.get().forceGetComponent<SpriteAnimator>();
}

fro::Reference<fro::State> fro::DigDugPumpState::update(float const deltaSeconds)
{
	if (m_RigidBody.valid() and m_RigidBody.get().isOverlapping("enemy"))
		return parentingGameObject.get().forceGetComponent<DigDugDeadState>();

	InputManager& inputManager{ InputManager::getInstance() };
	glm::vec2 const& actionStrength{ inputManager.getActionStrengthAxis2D("moveRightDigDug", "moveLeftDigDug", "moveUpDigDug", "moveDownDigDug") };

	if (actionStrength.x or actionStrength.y)
		return parentingGameObject.get().forceGetComponent<DigDugMoveState>();

	m_RemainingCooldownSeconds -= deltaSeconds;

	if (inputManager.isActionJustPressed("attackDigDug"))
		m_RemainingCooldownSeconds -= 0.08f;

	if (not inputManager.getActionStrength("attackDigDug"))
	{
		m_ElapsedSecondsWithoutInput += deltaSeconds;
		if (m_ElapsedSecondsWithoutInput >= m_IdleTimeoutSeconds)
			return parentingGameObject.get().forceGetComponent<DigDugIdleState>();
	}
	else if (m_RemainingCooldownSeconds <= 0.0f)
	{
		m_RemainingCooldownSeconds = m_CooldownSeconds;

		m_AudioService.stopEffect("Sounds/Dig Dug Pumping.mp3");
		m_AudioService.playEffect("Sounds/Dig Dug Pumping.mp3");

		if (m_SpriteAnimator.valid())
		{
			m_SpriteAnimator.get().reset();
			m_SpriteAnimator.get().play();
		}

		m_ElapsedSecondsWithoutInput = 0.0f;
	}

	return {};
}

void fro::DigDugPumpState::enter(Reference<State> const)
{
	if (m_SpriteAnimator.valid())
		m_SpriteAnimator.get().setActiveAnimation("pumping");
}

void fro::DigDugPumpState::exit(Reference<State> const)
{
	m_AudioService.stopEffect("Sounds/Dig Dug Pumping.mp3");

	m_ElapsedSecondsWithoutInput = 0.0f;
	m_RemainingCooldownSeconds = 0.0f;
}
#pragma endregion PublicMethods