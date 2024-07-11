#include "froch.hpp"

#include "Logger/Logger.hpp"
#include "GlobalEventManager/GlobalEventManager.hpp"
#include "Window/Window.hpp"
#include "WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	Window::Implementation::Implementation(std::string_view const windowTitle, int const width, int const height)
	{
		mSDLWindow = { SDL_CreateWindow(windowTitle.data(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height,
			SDL_WINDOW_RESIZABLE), SDL_DestroyWindow };

		FRO_ASSERT(mSDLWindow.get(), "failed to create Window ({})", SDL_GetError());
		Logger::info("created a {}x{} Window titled \"{}\" with ID {}!",
			width, height, windowTitle, getID());
	}

	SDL_Window* Window::Implementation::getSDLWindow() const
	{
		return mSDLWindow.get();
	}

	std::uint32_t Window::Implementation::getID() const
	{
		return SDL_GetWindowID(mSDLWindow.get());
	}

	Window::Window(std::string_view const title, int const width, int const height)
		: mTitle{ title }
		, mWidth{ width }
		, mHeight{ height }
		, mImplementation{ std::make_unique<Implementation>(mTitle, mWidth, mHeight) }
		, mID{ mImplementation->getID() }
	{
		GlobalEventManager::mWindowCloseEvent.addListener(mOnWindowCloseEvent);
		GlobalEventManager::mWindowResizeEvent.addListener(mOnWindowResizeEvent);
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

	std::uint32_t Window::getID() const
	{
		return mID;
	}

	int Window::getWidth() const
	{
		return mWidth;
	}

	int Window::getHeight() const
	{
		return mHeight;
	}
}