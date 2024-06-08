#include "DigDugDeadState.h"

#include "SceneManager.h"
#include "SpriteAnimator.h"
#include "GameObject.h"

#pragma region Constructors/Destructor
fro::DigDugDeadState::DigDugDeadState(Reference<GameObject> const parentinGameObject)
	: State(parentinGameObject)
{
}
#pragma endregion Constructors/Destructor


#pragma region PublicMethods
void fro::DigDugDeadState::awake()
{
	m_SpriteAnimator = parentingGameObject.get().forceGetComponent<SpriteAnimator>();

	m_Pump = parentingGameObject.get().forceGetGameObject("pump");
}

fro::Reference<fro::State> fro::DigDugDeadState::fixedUpdate(float const)
{
	if (not m_SpriteAnimator.valid())
		return {};

	if (m_SpriteAnimator.get().getAnimationProgress() == 1.0f)
		SceneManager::getInstance().removeScene(parentingGameObject.get().getParentingScene().get().getName());

	return {};
}

void fro::DigDugDeadState::enter(Reference<State> const)
{
	if (m_Pump.valid())
		m_Pump.get().setActive(false);

	if (m_SpriteAnimator.valid())
	{
		m_SpriteAnimator.get().setActiveAnimation("dead");
		m_SpriteAnimator.get().play();
	}

	m_AudioService.playEffect("Sounds/Player Death.mp3");
}
#pragma endregion PublicMethods