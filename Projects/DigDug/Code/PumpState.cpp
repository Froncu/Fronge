#include "PumpState.h"

#include "GameObject.h"
#include "IdleState.h"
#include "InputManager.h"
#include "MoveState.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::PumpState::PumpState(Reference<GameObject> const parentingGameObject)
	: Component(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
std::unique_ptr<fro::State> fro::PumpState::update(float const deltaSeconds)
{
	InputManager& inputManager{ InputManager::getInstance() };
	glm::vec2 const& actionStrength{ inputManager.getActionStrengthAxis2D("moveRight1", "moveLeft1", "moveUp1", "moveDown1") };

	if (actionStrength.x or actionStrength.y)
		return std::make_unique<MoveState>(m_ParentingGameObject);

	if (not inputManager.getActionStrength("attack"))
	{
		m_ElapsedSecondsWithoutInput += deltaSeconds;
		if (m_ElapsedSecondsWithoutInput >= 0.75f)
			return std::make_unique<IdleState>(m_ParentingGameObject);
	}
	else
	{
		m_ParentingGameObject.get().getComponent<SpriteAnimator>()->play();
		m_ElapsedSecondsWithoutInput = 0.0f;
	}

	return nullptr;
}

void fro::PumpState::enter(std::unique_ptr<State> const&)
{
	SpriteAnimator& spriteAnimator{ *m_ParentingGameObject.get().getComponent<SpriteAnimator>()};
	spriteAnimator.setActiveAnimation("pumping");
	spriteAnimator.play();
}

void fro::PumpState::exit(std::unique_ptr<State> const&)
{
	for (Reference<GameObject> const child : m_ParentingGameObject.get().getChildren())
		child.get().setActive(false);
}
#pragma endregion PublicMethods