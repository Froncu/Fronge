#include "froch.hpp"

#include "SystemEventManager/SystemEventManager.hpp"
#include "Window.hpp"
#include "Window/Implementation/WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	Window::Window(std::string_view const title, Vector2<int> const size)
		: mTitle{ title }
		, mSize{ size }
		, mImplementation{ std::make_unique<Implementation>(mTitle, getSize()) }
		, mID{ mImplementation->getID() }
	{
		SystemEventManager::mWindowEvent.addListener(mOnWindowEvent);
	}

	Window::~Window()
	{
		Logger::info("destroyed Window with ID {}!",
			mID);
	}

	Window::Implementation& Window::getImplementation() const
	{
		return *mImplementation;
	}

	bool Window::setFullscreen(bool const fullscreen)
	{
		int result;
		if (fullscreen)
		{
			result = SDL_SetWindowFullscreen(mImplementation->getSDLWindow(), SDL_WINDOW_FULLSCREEN_DESKTOP);

			if (result == 0)
				Logger::info("set Window with ID {} to fullscreen!",
					mID);
			else
				Logger::warn("failed to set Window with ID {} to fullscreen ({})",
					mID, SDL_GetError());
		}
		else
		{
			result = SDL_SetWindowFullscreen(mImplementation->getSDLWindow(), 0);

			if (result == 0)
				Logger::info("set Window with ID {} to windowed!",
					mID);
			else
				Logger::warn("failed to set Window with ID {} to windowed ({})",
					mID, SDL_GetError());
		}

		return result;
	}

	std::uint32_t Window::getID() const
	{
		return mID;
	}

	Vector2<int> Window::getSize() const
	{
		return mSize;
	}
}