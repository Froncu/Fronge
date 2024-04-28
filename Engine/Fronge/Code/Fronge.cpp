#include "Fronge.h"

#include "GUIContext.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "RenderContext.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Steam.h"
#include "SystemEventManager.h"
#include "Timer.h"

#include <sdl.h>
#include <vld.h>

#pragma region Constructors/Destructor
fro::Fronge::Fronge()
{
	SystemEventManager::getInstance().m_SystemEvent.addSubscribers({
		std::bind(&InputManager::processInputEvent, &InputManager::getInstance(), std::placeholders::_1),
		std::bind(&GUIContext::processSystemEvent, &GUIContext::getInstance(), std::placeholders::_1) });
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
		Steam::getInstance().update();

		Timer::getInstance().update();

		InputManager::getInstance().processKeyboardInputContinous();
		InputManager::getInstance().processGamePadInputContinous();
		if (!SystemEventManager::getInstance().processSystemEvents())
			break;

		PhysicsManager::getInstance().update(Timer::getInstance().getDeltaSeconds());

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