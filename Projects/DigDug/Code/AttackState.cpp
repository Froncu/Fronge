#include "AttackState.h"

#include "InputManager.h"
#include "PumpState.h"

#pragma region Constructors/Destructor
fro::AttackState::AttackState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::AttackState::update(float const deltaSeconds)
{
	Reference<GameObject> const pump{ m_ParentingGameObject.get().getChild("pump") };

	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds >= 0.5f)
		return m_ParentingGameObject.get().forceGetComponent<PumpState>();

	return {};
}

void fro::AttackState::enter(Reference<State> const)
{
	m_AudioService.playEffect("Sounds/Dig Dug Shot.mp3");

	Reference<GameObject> const pump{ m_ParentingGameObject.get().getChild("pump")};
	pump.get().setActive(true);

	Reference<SpriteAnimator> spriteAnimator{ pump.get().forceGetComponent<SpriteAnimator>() };
	spriteAnimator.get().reset();
	spriteAnimator.get().play();

	m_SpriteAnimator.get().setActiveAnimation("attacking");
}

void fro::AttackState::exit(Reference<State> const)
{
	m_AudioService.stopEffect("Sounds/Dig Dug Shot.mp3");

	m_ElapsedSeconds = 0.0f;

	Reference<GameObject> pump{ m_ParentingGameObject.get().getChild("pump") };
	pump.get().getComponent<SpriteAnimator>().get().pause();

	m_SpriteAnimator.get().pause();
}
#pragma endregion PublicMethods