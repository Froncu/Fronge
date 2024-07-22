#include "froch.hpp"

#include "Logger/Logger.hpp"
#include "SystemEventManager/SystemEventManager.hpp"
#include "WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	Window::Implementation::Implementation(std::string_view const windowTitle, Vector2<int> const size)
		: mSDLWindow{ SDL_CreateWindow(windowTitle.data(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			size.x, size.y,
			SDL_WINDOW_RESIZABLE), SDL_DestroyWindow }
	{
		FRO_ASSERT(mSDLWindow.get(), "failed to create Window ({})", SDL_GetError());
		Logger::info("created a {}x{} Window titled \"{}\" with ID {}!",
			size.x, size.y, windowTitle, getID());
	}

	SDL_Window* Window::Implementation::getSDLWindow() const
	{
		return mSDLWindow.get();
	}

	std::uint32_t Window::Implementation::getID() const
	{
		return SDL_GetWindowID(mSDLWindow.get());
	}

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