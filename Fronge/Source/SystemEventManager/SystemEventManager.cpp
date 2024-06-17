#include "froch.hpp"

#include "SystemEventManager.hpp"

#include <SDL.h>

namespace fro
{
	EventDispatcher<std::uint32_t> SystemEventManager::mOnWindowClose{};

	void SystemEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_WINDOWEVENT)
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
					mOnWindowClose.notify(event.window.windowID);
	}
}