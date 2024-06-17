#include "SystemEventManager.hpp"

#include <SDL.h>

namespace fro
{
	EventDispatcher<WindowCloseEvent&> SystemEventManager::mOnWindowClose{}; 

	void SystemEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_WINDOWEVENT)
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					WindowCloseEvent windowCloseEvent{ event.window.windowID };
					mOnWindowClose.notify(windowCloseEvent);
				}
	}
}