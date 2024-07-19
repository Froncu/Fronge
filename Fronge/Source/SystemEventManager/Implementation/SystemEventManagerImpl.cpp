#include "froch.hpp"

#include "InputManager/Input/Implementation/InputImpl.hpp"
#include "SystemEventManagerImpl.hpp"
#include "Logger/Logger.hpp"

#include <SDL.h>

namespace fro
{
	void SystemEventManager::Implementation::dispatchSDLWindowEvent(SDL_WindowEvent const& SDLWindowEvent)
	{
		switch (SDLWindowEvent.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			WindowCloseEvent windowCloseEvent{ SDLWindowEvent.windowID };
			Logger::info("dispatched {}!", windowCloseEvent.getLogString());

			mWindowEvent.notify(std::move(windowCloseEvent));
			break;
		}
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			WindowResizeEvent windowResizeEvent
			{
				.ID{ SDLWindowEvent.windowID },
				.size{ SDLWindowEvent.data1, SDLWindowEvent.data2 }
			};

			Logger::info("dispatched {}!", windowResizeEvent.getLogString());

			mWindowEvent.notify(std::move(windowResizeEvent));
			break;
		}
		}
	}

	void SystemEventManager::Implementation::dispatchSDLKeyboardEvent(SDL_KeyboardEvent const& SDLKeyboardEvent)
	{
		switch (SDLKeyboardEvent.type)
		{
		case SDL_KEYDOWN:
		{
			KeyDownEvent keyDownEvent{ convertSDLKeyCode(SDLKeyboardEvent.keysym.sym) };

			mInputEvent.notify(std::move(keyDownEvent));
			break;
		}
		case SDL_KEYUP:
		{
			KeyUpEvent keyUpEvent{ convertSDLKeyCode(SDLKeyboardEvent.keysym.sym) };

			mInputEvent.notify(std::move(keyUpEvent));
			break;
		}
		}
	}

	void SystemEventManager::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				Implementation::dispatchSDLWindowEvent(event.window);
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				// TODO: no support for repeated key down events
				if (event.key.repeat == 0)
					Implementation::dispatchSDLKeyboardEvent(event.key);
			}
	}

	EventDispatcher<WindowEvent const> SystemEventManager::mWindowEvent{};
	EventDispatcher<InputEvent const> SystemEventManager::mInputEvent{};
}