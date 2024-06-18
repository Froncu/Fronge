#include "froch.hpp"

#include "GlobalEventManager.hpp"
#include "Logger.hpp"

#include <SDL.h>

namespace fro
{
	Event<BaseEvent&> GlobalEventManager::mSystem{};
	Event<WindowEvent&> GlobalEventManager::mWindow{};
	Event<WindowCloseEvent&> GlobalEventManager::mWindowClose{};

	void GlobalEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_WINDOWEVENT)
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					WindowCloseEvent windowCloseEvent{ event.window.windowID };
					Logger::info("{}!", windowCloseEvent.getLogString());

					mWindowClose.notify(windowCloseEvent);
					mWindow.notify(windowCloseEvent);
					mSystem.notify(windowCloseEvent);
				}
	}
}