#include "froch.hpp"

#include "GlobalEventManager.hpp"
#include "Logger.hpp"

#include <SDL.h>

namespace fro
{
	EventDispatcher<Event&> GlobalEventManager::mSystemEvent{};
	EventDispatcher<WindowEvent&> GlobalEventManager::mWindowEvent{};
	EventDispatcher<WindowCloseEvent&> GlobalEventManager::mWindowCloseEvent{};

	void GlobalEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_WINDOWEVENT)
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					WindowCloseEvent windowCloseEvent{ event.window.windowID };
					Logger::info("{}!", windowCloseEvent.getLogString());

					mWindowCloseEvent.notify(windowCloseEvent);
					if (windowCloseEvent.isHandeled())
						continue;

					mWindowEvent.notify(windowCloseEvent);
					if (windowCloseEvent.isHandeled())
						continue;

					mSystemEvent.notify(windowCloseEvent);
				}
	}
}