#include "EventManager.h"

#include "GUIContext.h"
#include "InputManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdexcept>
#include <format>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(EventManager)
{
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0)
		throw std::runtime_error(std::format("[ SDL_InitSubSystem() FAILED ] -> {}", SDL_GetError()));
}

fro_GENERATED_SINGLETON_DESTRUCTOR(EventManager)
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
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

		GUIContext::getInstance().processEvent(event);
	}

	return true;
}
#pragma endregion PublicMethods