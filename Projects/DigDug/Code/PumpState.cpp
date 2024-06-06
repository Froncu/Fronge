#include "PumpState.h"

#include "GameObject.h"
#include "IdleState.h"
#include "InputManager.h"
#include "MoveState.h"

#pragma region Constructors/Destructor
fro::PumpState::PumpState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::PumpState::update(float const deltaSeconds)
{
	InputManager& inputManager{ InputManager::getInstance() };
	glm::vec2 const& actionStrength{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };

	if (actionStrength.x or actionStrength.y)
		return m_ParentingGameObject.get().forceGetComponent<MoveState>();

	m_RemainingCooldownSeconds -= deltaSeconds;

	if (not inputManager.getActionStrength("attack"))
	{
		m_ElapsedSecondsWithoutInput += deltaSeconds;
		if (m_ElapsedSecondsWithoutInput >= m_IdleTimeoutSeconds)
			return m_ParentingGameObject.get().forceGetComponent<IdleState>();
	}
	else if (m_RemainingCooldownSeconds <= 0.0f)
	{
		m_RemainingCooldownSeconds = m_CooldownSeconds;

		m_AudioService.stopEffect("Sounds/Dig Dug Pumping.mp3");
		m_AudioService.playEffect("Sounds/Dig Dug Pumping.mp3");

		m_SpriteAnimator.get().reset();
		m_SpriteAnimator.get().play();

		m_ElapsedSecondsWithoutInput = 0.0f;
	}

	return {};
}

void fro::PumpState::enter(Reference<State> const)
{
	m_SpriteAnimator.get().setActiveAnimation("pumping");
}

void fro::PumpState::exit(Reference<State> const)
{
	m_AudioService.stopEffect("Sounds/Dig Dug Pumping.mp3");

	m_ElapsedSecondsWithoutInput = 0.0f;
	m_RemainingCooldownSeconds = 0.0f;

	m_ParentingGameObject.get().getChild("pump").get().setActive(false);
}
#pragma endregion PublicMethods