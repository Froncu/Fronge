#include "EventManager.h"

#include <SDL2/SDL_events.h>

#pragma region PublicMethods
bool fro::EventManager::processEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		if (event.type == SDL_EventType::SDL_QUIT)
			return false;

	return true;
}
#pragma endregion PublicMethods