#include "froch.hpp"

#include "SystemEventManager.hpp"
#include "Implementation/SystemEventManagerImpl.hpp"

#include <SDL.h>

namespace fro
{
	void SystemEventManager::initialize()
	{
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

		Logger::info("initialized SystemEventManager!");
	}

	void SystemEventManager::shutDown()
	{
		SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

		Logger::info("shut down SystemEventManager!");
	}

	void SystemEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				Implementation::dispatchSDLWindowEvent(event.window);
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				// TODO: no support for repeated key down events
				if (event.key.repeat == 0)
					Implementation::dispatchSDLKeyboardEvent(event.key);
				break;

			case SDL_CONTROLLERDEVICEADDED:
			case SDL_CONTROLLERDEVICEREMOVED:
				Implementation::dispatchSDLControllerDeviceEvent(event.cdevice);
				break;

			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				Implementation::dispatchSDLControllerButtonEvent(event.cbutton);
				break;

			case SDL_CONTROLLERAXISMOTION:
				Implementation::dispatchSDLControllerAxisEvent(event.caxis);
				break;
			}
	}

	EventDispatcher<WindowEvent const> SystemEventManager::mWindowEvent{};
	EventDispatcher<InputEvent const> SystemEventManager::mInputEvent{};
}