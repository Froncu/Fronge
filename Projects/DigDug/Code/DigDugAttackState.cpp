#include "DigDugAttackState.h"

#include "DigDugDeadState.h"
#include "DigDugIdleState.h"
#include "DigDugPumpState.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SpriteAnimator.h"
#include "RigidBody.h"

#pragma region Constructors/Destructor
fro::DigDugAttackState::DigDugAttackState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::DigDugAttackState::awake()
{
	m_RigidBody = parentingGameObject.get().forceGetComponent<RigidBody>();

	m_SpriteAnimator = parentingGameObject.get().forceGetComponent<SpriteAnimator>();

	m_Pump = parentingGameObject.get().getGameObject("pump");
	m_PumpSpriteAnimator = m_Pump.get().forceGetComponent<fro::SpriteAnimator>();

	m_PumpHitBox = parentingGameObject.get().forceGetGameObject("pumpHitBox");
}

fro::Reference<fro::State> fro::DigDugAttackState::fixedUpdate(float const)
{
	if (m_RigidBody.valid() and m_RigidBody.get().isOverlapping("enemy"))
		return parentingGameObject.get().forceGetComponent<DigDugDeadState>();

	auto const& animation{ m_PumpSpriteAnimator.get().getAnimation("shooting") };

	float const animationProgress{ m_PumpSpriteAnimator.get().getAnimationProgress() };

	if (m_PumpHitBox.valid())
	{
		Reference<RigidBody> const overlappingEnemy{ m_PumpHitBox.get().forceGetComponent<RigidBody>().get().getOverlapping("enemy") };
		if (overlappingEnemy.valid())
		{
			Reference<DigDugPumpState> pumpState{ parentingGameObject.get().forceGetComponent<DigDugPumpState>() };
			pumpState.get().m_AttackedEnemy = overlappingEnemy.get().parentingGameObject;
			return pumpState;
		}
		else if (animationProgress == 1.0f)
			return parentingGameObject.get().forceGetComponent<DigDugIdleState>();
	}

	float const travelWidth{ animation.vAnimationFrames.front().sourceRectangle.w };
	if (m_PumpHitBox.valid())
		m_PumpHitBox.get().setLocalTranslation(
			{
				travelWidth * m_PumpSpriteAnimator.get().getAnimationProgress() - travelWidth / 2,
				0.0f
			});

	return {};
}

void fro::DigDugAttackState::enter(Reference<State> const)
{
	m_AudioService.playEffect("Sounds/Dig Dug Shot.mp3");

	if (m_Pump.valid())
		m_Pump.get().setActive(true);

	if (m_PumpSpriteAnimator.valid())
	{
		m_PumpSpriteAnimator.get().reset();
		m_PumpSpriteAnimator.get().play();
	}

	if (m_SpriteAnimator.valid())
		m_SpriteAnimator.get().setActiveAnimation("attacking");
}

void fro::DigDugAttackState::exit(Reference<State> const)
{
	m_AudioService.stopEffect("Sounds/Dig Dug Shot.mp3");

	if (m_PumpSpriteAnimator.valid())
		m_PumpSpriteAnimator.get().pause();

	if (m_SpriteAnimator.valid())
		m_SpriteAnimator.get().pause();
}
#pragma endregion PublicMethods