#include "EventManager.h"

#include "GUI.h"
#include "InputManager.h"

#include <SDL2/SDL_events.h>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(EventManager)
{
}

fro_GENERATED_SINGLETON_DESTRUCTOR(EventManager)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
bool fro::EventManager::processEvents() const
{
	InputManager::getInstance().processKeyboardInputContinous();
	InputManager::getInstance().processGamePadInputContinous();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EventType::SDL_QUIT)
			return false;

		InputManager::getInstance().processInputEvent(event);

		GUI::getInstance().processEvent(event);
	}

	return true;
}
#pragma endregion PublicMethods