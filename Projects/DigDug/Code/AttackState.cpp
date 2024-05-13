#include "AttackState.h"

#include "GameObject.h"
#include "PumpState.h"
#include "InputManager.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::AttackState::AttackState(GameObject const& parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
std::unique_ptr<fro::State> fro::AttackState::update(float const deltaSeconds)
{
	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds >= 0.5f)
		return std::make_unique<PumpState>(getParentingGameObject());

	return nullptr;
}

void fro::AttackState::enter(std::unique_ptr<State> const&)
{
	for (GameObject const* pChild : getParentingGameObject().getChildren())
	{
		pChild->setActive(true);

		SpriteAnimator& spriteAnimator{ *pChild->getComponent<SpriteAnimator>() };
		spriteAnimator.reset();
		spriteAnimator.play();
	}

	SpriteAnimator& spriteAnimator{ *getParentingGameObject().getComponent<SpriteAnimator>()};
	spriteAnimator.setActiveAnimation("attacking");
}

void fro::AttackState::exit(std::unique_ptr<State> const&)
{
	for (GameObject const* pChild : getParentingGameObject().getChildren())
	{
		SpriteAnimator& spriteAnimator{ *pChild->getComponent<SpriteAnimator>() };
		spriteAnimator.pause();
	}
}
#pragma endregion PublicMethods