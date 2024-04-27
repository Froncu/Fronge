#include "Console.hpp"
#include "Fronge.h"
#include "GameObject.h"
#include "Rotator.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "Transform.h"

#include <sdl_main.h>

#include <stdexcept>

int main(int, char**)
{
	try
	{
		fro::Scene& scene{ fro::SceneManager::getInstance().addScene() };

		fro::GameObject* pGameObject;
		pGameObject = &scene.addGameObject();
		pGameObject->addComponent<fro::Sprite>()->setFileName("logo.tga");
		pGameObject->getComponent<fro::Transform>()->setLocalPosition({ 320, 240 });

		for (std::size_t index{}; index < 500; ++index)
		{
			float constexpr basePeriod{ 2.0f };
			float constexpr childPeriodDelayMultiplier{ 0.002f };

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
	catch (std::runtime_error const& exception)
	{
		fro::Console::getInstance().log(exception.what(), fro::Console::BackgroundColor::DarkRed);
		return 1;
	}
}