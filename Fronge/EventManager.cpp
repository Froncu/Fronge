#include "EventManager.h"

#include "GUI.h"
#include "InputManager.h"

#include <SDL2/SDL_events.h>

#pragma region PublicMethods
bool fro::EventManager::processEvents()
{
	InputManager::processKeyboardInputContinous();
	InputManager::processGamePadInputContinous();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EventType::SDL_QUIT)
			return false;

		InputManager::processInputEvent(event);

		GUI::processEvent(event);
	}

	return true;
}
#pragma endregion PublicMethods