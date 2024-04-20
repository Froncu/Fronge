#include "SystemEventManager.h"

#include <SDL.h>
#include <SDL_events.h>
#include <stdexcept>
#include <format>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(SystemEventManager)
{
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0)
		throw std::runtime_error(std::format("[ SDL_InitSubSystem() FAILED ] -> {}", SDL_GetError()));
}

fro_GENERATED_SINGLETON_DESTRUCTOR(SystemEventManager)
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
bool fro::SystemEventManager::processSystemEvents() const
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EventType::SDL_QUIT)
			return false;

		m_SystemEvent.notifySubscribers(event);
	}

	return true;
}
#pragma endregion PublicMethods