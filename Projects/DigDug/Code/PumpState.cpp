#include "PumpState.h"

#include "GameObject.h"
#include "IdleState.h"
#include "InputManager.h"
#include "MoveState.h"
#include "SpriteAnimator.h"

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

	if (not inputManager.getActionStrength("attack"))
	{
		m_ElapsedSecondsWithoutInput += deltaSeconds;
		if (m_ElapsedSecondsWithoutInput >= 0.75f)
			return m_ParentingGameObject.get().forceGetComponent<IdleState>();
	}
	else
	{
		m_ParentingGameObject.get().getComponent<SpriteAnimator>().get().play();
		m_ElapsedSecondsWithoutInput = 0.0f;
	}

	return {};
}

void fro::PumpState::enter(Reference<State> const)
{
	Reference<SpriteAnimator> spriteAnimator{ m_ParentingGameObject.get().getComponent<SpriteAnimator>()};
	spriteAnimator.get().setActiveAnimation("pumping");
	spriteAnimator.get().play();
}

void fro::PumpState::exit(Reference<State> const)
{
	m_ElapsedSecondsWithoutInput = 0.0f;

	for (Reference<GameObject> const child : m_ParentingGameObject.get().getChildren())
		child.get().setActive(false);
}
#pragma endregion PublicMethods