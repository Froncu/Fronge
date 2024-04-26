#include "Fronge.h"

#include "SystemEventManager.h"
#include "InputManager.h"
#include "GUIContext.h"
#include "ServiceLocator.hpp"
#include "AudioService.h"
#include "Steam.h"
#include "Timer.h"
#include "SceneManager.h"
#include "RenderContext.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <vld.h>

#pragma region Constructors/Destructor
fro::Fronge::Fronge()
{
	SystemEventManager::getInstance().m_SystemEvent.addSubscribers
	({
		std::bind(&InputManager::processInputEvent, &InputManager::getInstance(), std::placeholders::_1),
		std::bind(&GUIContext::processSystemEvent, &GUIContext::getInstance(), std::placeholders::_1)
	});
}

fro::Fronge::~Fronge()
{
	SDL_Quit();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
int fro::Fronge::run()
{
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