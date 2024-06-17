#include "SystemEventManager.hpp"

#include <SDL.h>

namespace fro
{
	void SDLToFrongeEvent(SDL_Event const)
	{
	}

	void SystemEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{

		}
	}
}