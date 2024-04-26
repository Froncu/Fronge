#include "Fronge.h"

#include "SystemEventManager.h"
#include "SceneManager.h"
#include "GUIContext.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Rotator.h"
#include "Text.h"
#include "FPSCounter.h"
#include "Plot.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "Console.h"
#include "RenderContext.h"
#include "Steam.h"
#include "ServiceLocator.hpp"
#include "AudioService.h"

#include <SDL.h>
#include <vld.h>
#include <cassert>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(Fronge)
{
}

fro_GENERATED_SINGLETON_DESTRUCTOR(Fronge)
{
	SDL_Quit();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
int fro::Fronge::run()
{
	SystemEventManager::getInstance().m_SystemEvent.addSubscribers
	({
		std::bind(&InputManager::processInputEvent, &InputManager::getInstance(), std::placeholders::_1),
		std::bind(&GUIContext::processSystemEvent, &GUIContext::getInstance(), std::placeholders::_1)
	});

	while (true)
	{
		ServiceLocator<AudioService>::getInstance().getService().update();

		Steam::getInstance().update();

		Timer::getInstance().update();

		InputManager::getInstance().processKeyboardInputContinous();
		InputManager::getInstance().processGamePadInputContinous();
		if (!SystemEventManager::getInstance().processSystemEvents())
			break;

		SceneManager::getInstance().update();

		RenderContext::getInstance().clear();
		SceneManager::getInstance().render();

		GUIContext::getInstance().startFrame();
		SceneManager::getInstance().display();

		GUIContext::getInstance().endFrame();
		RenderContext::getInstance().present();
	}

	return 0;
}
#pragma endregion PublicMethods