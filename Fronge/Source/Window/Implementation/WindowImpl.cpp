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

		FRO_ASSERT(mSDLWindow.get(), "failed to create window ({})", SDL_GetError());
		Logger::info("a {}x{} \"{}\" window with ID {} created!",
			width, height, windowTitle, getID());
	}

	SDL_Window* Window::Implementation::getSDLWindow() const
	{
		return mSDLWindow.get();
	}

	std::string_view Window::Implementation::getTitle() const
	{
		return SDL_GetWindowTitle(mSDLWindow.get());
	}

	std::uint32_t Window::Implementation::getID() const
	{
		return SDL_GetWindowID(mSDLWindow.get());
	}

	Window::Window(std::string_view const title, int const width, int const height)
		: mImplementation{ std::make_unique<Implementation>(title, width, height) }
		, mTitle{ mImplementation->getTitle() }
		, mWidth{ width }
		, mHeight{ height }
		, mID{ mImplementation->getID() }
	{
		GlobalEventManager::mWindowCloseEvent.addListener(mOnWindowCloseEvent);
		GlobalEventManager::mWindowResizeEvent.addListener(mOnWindowResizeEvent);
	}

	Window::~Window()
	{
	}

	Window::Implementation& Window::getImplementation() const
	{
		return *mImplementation;
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