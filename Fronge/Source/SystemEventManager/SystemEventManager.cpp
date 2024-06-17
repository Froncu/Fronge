#include "froch.hpp"

#include "SystemEventManager.hpp"

#include <SDL.h>

namespace fro
{
	Event<std::uint32_t> SystemEventManager::mWindowClose{};

	void SystemEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_WINDOWEVENT)
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
					mWindowClose.notify(event.window.windowID);
	}
}