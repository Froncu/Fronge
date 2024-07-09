#include "froch.hpp"

#include "GlobalEventManager.hpp"
#include "Logger/Logger.hpp"

#include <SDL.h>

namespace fro
{
	EventDispatcher<WindowCloseEvent&> GlobalEventManager::mWindowCloseEvent{};
	EventDispatcher<WindowResizeEvent&> GlobalEventManager::mWindowResizeEvent{};

	void GlobalEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_WINDOWEVENT)
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
				{
					WindowCloseEvent windowCloseEvent{ event.window.windowID };
					Logger::info("{}!", windowCloseEvent.getLogString());

					mWindowCloseEvent.notify(windowCloseEvent);
					break;
				}
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					WindowResizeEvent windowResizeEvent{ event.window.windowID, event.window.data1, event.window.data2 };
					Logger::info("{}!", windowResizeEvent.getLogString());

					mWindowResizeEvent.notify(windowResizeEvent);
					break;
				}
				}
	}
}