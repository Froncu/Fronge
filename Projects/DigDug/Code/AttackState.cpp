#include "AttackState.h"

#include "Console.hpp"
#include "IdleState.h"
#include "InputManager.h"
#include "PumpState.h"

#pragma region Constructors/Destructor
fro::AttackState::AttackState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::AttackState::fixedUpdate(float const)
{
	auto const& animation{ m_PumpSpriteAnimator.get().getAnimation("shooting") };

	float const animationProgress{ m_PumpSpriteAnimator.get().getAnimationProgress() };

	if (m_PumpHitBox.get().forceGetComponent<RigidBody>().get().isOverlapping("enemy"))
		return parentingGameObject.get().forceGetComponent<PumpState>();
	else if (animationProgress == 1.0f)
		return parentingGameObject.get().forceGetComponent<IdleState>();

	float const travelWidth{ animation.vAnimationFrames.front().sourceRectangle.w };
	m_PumpHitBox.get().setLocalTranslation(
		{
			travelWidth * m_PumpSpriteAnimator.get().getAnimationProgress() - travelWidth / 2,
			0.0f
		});

	return {};
}

void fro::AttackState::enter(Reference<State> const)
{
	m_AudioService.playEffect("Sounds/Dig Dug Shot.mp3");

	m_Pump.get().setActive(true);

	m_PumpSpriteAnimator.get().reset();
	m_PumpSpriteAnimator.get().play();

	m_SpriteAnimator.get().setActiveAnimation("attacking");
}

void fro::AttackState::exit(Reference<State> const)
{
	m_AudioService.stopEffect("Sounds/Dig Dug Shot.mp3");

	m_Pump.get().getComponent<SpriteAnimator>().get().pause();

	m_SpriteAnimator.get().pause();
}
#pragma endregion PublicMethods