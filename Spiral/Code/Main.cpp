#include "Fronge.h"
#include "GameObject.h"
#include "Rotator.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "Transform.h"

#include <sdl_main.h>

int main(int, char**)
{
	fro::Scene& scene{ fro::SceneManager::getInstance().addScene() };

	fro::GameObject* pGameObject;
	pGameObject = &scene.addGameObject();
	pGameObject->addComponent<fro::Sprite>()->setFileName("logo.tga");
	pGameObject->getComponent<fro::Transform>()->setLocalPosition({ 320, 240 });

	for (std::size_t index{}; index < 500; ++index)
	{
		constexpr float basePeriod{ 2.0f };
		constexpr float childPeriodDelayMultiplier{ 0.002f };

		fro::GameObject& gameObject{ scene.addGameObject() };
		gameObject.addComponent<fro::Sprite>()->setFileName("logo.tga");
		fro::Rotator* const pRotator{ gameObject.addComponent<fro::Rotator>() };
		pRotator->setPeriod(basePeriod + index * childPeriodDelayMultiplier);
		pRotator->setRadius(2.0f);

		gameObject.setParent(pGameObject, false);
		pGameObject = &gameObject;
	}

	return fro::Fronge::getInstance().run();
}