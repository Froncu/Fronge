#include "DeadState.h"

#include "SceneManager.h"

#pragma region Constructors/Destructor
fro::DeadState::DeadState(Reference<GameObject> const parentinGameObject)
	: State(parentinGameObject)
{
}
#pragma endregion Constructors/Destructor


#pragma region PublicMethods
fro::Reference<fro::State> fro::DeadState::fixedUpdate(float const)
{
	if (m_SpriteAnimator.get().getAnimationProgress() == 1.0f)
		SceneManager::getInstance().removeScene(parentingGameObject.get().getParentingScene().get().getName());

	return {};
}

void fro::DeadState::enter(Reference<State> const)
{
	m_Pump.get().setActive(false);

	m_SpriteAnimator.get().setActiveAnimation("dead");
	m_SpriteAnimator.get().play();

	m_AudioService.playEffect("Sounds/Player Death.mp3");
}
#pragma endregion PublicMethods