#include "AttackState.h"

#include "GameObject.h"
#include "PumpState.h"
#include "InputManager.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::AttackState::AttackState(Reference<GameObject> const parentingGameObject)
	: Component(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
std::unique_ptr<fro::State> fro::AttackState::update(float const deltaSeconds)
{
	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds >= 0.5f)
		return std::make_unique<PumpState>(m_ParentingGameObject);

	return nullptr;
}

void fro::AttackState::enter(std::unique_ptr<State> const&)
{
	for (Reference<GameObject> const child : m_ParentingGameObject.get().getChildren())
	{
		child.get().setActive(true);

		Reference<SpriteAnimator> spriteAnimator{ child.get().getComponent<SpriteAnimator>() };
		spriteAnimator.get().reset();
		spriteAnimator.get().play();
	}

	Reference<SpriteAnimator> spriteAnimator{ m_ParentingGameObject.get().getComponent<SpriteAnimator>()};
	spriteAnimator.get().setActiveAnimation("attacking");
}

void fro::AttackState::exit(std::unique_ptr<State> const&)
{
	for (Reference<GameObject> const child : m_ParentingGameObject.get().getChildren())
	{
		Reference<SpriteAnimator> spriteAnimator{ child.get().getComponent<SpriteAnimator>()};
		spriteAnimator.get().pause();
	}
}
#pragma endregion PublicMethods