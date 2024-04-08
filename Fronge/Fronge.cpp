#include "Fronge.h"

#include "SystemEventManager.h"
#include "InputManager.h"
#include "GUIContext.h"
#include "Steam.h"
#include "Timer.h"
#include "SceneManager.h"
#include "RenderContext.h"

#include <SDL2/SDL.h>

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