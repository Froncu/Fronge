#include "AttackState.h"

#include "GameObject.h"
#include "PumpState.h"
#include "InputManager.h"
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::AttackState::AttackState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::AttackState::update(float const deltaSeconds)
{
	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds >= 0.5f)
		return new PumpState(m_ParentingGameObject);
		//return m_ParentingGameObject.get().forceGetComponent<PumpState>();

	return {};
}

void fro::AttackState::enter(Reference<State> const)
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

void fro::AttackState::exit(Reference<State> const)
{
	m_ElapsedSeconds = 0.0f;

	for (Reference<GameObject> const child : m_ParentingGameObject.get().getChildren())
	{
		Reference<SpriteAnimator> spriteAnimator{ child.get().getComponent<SpriteAnimator>()};
		spriteAnimator.get().pause();
	}
}
#pragma endregion PublicMethods