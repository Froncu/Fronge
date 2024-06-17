#include "froch.hpp"

#include "Logger.hpp"
#include "SystemEventManager/SystemEventManager.hpp"
#include "Window/Window.hpp"
#include "WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	bool Window::Implementation::sSDLVideoInitialized{};

	Window::Implementation::Implementation(std::string_view const windowTitle, int const width, int const height)
	{
		if (not sSDLVideoInitialized)
		{
			Logger::info("initializing SDL_video...");
			[[maybe_unused]] int const failed{ SDL_InitSubSystem(SDL_INIT_VIDEO) };

			FRO_ASSERT(not failed, "failed to initialize SDL, {}", SDL_GetError());
			Logger::info("SDL_video initialized!");

			sSDLVideoInitialized = true;
		}

		Logger::info("creating a window...");
		mSDLWindow = { SDL_CreateWindow(windowTitle.data(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height,
			NULL), SDL_DestroyWindow };

		FRO_ASSERT(mSDLWindow.get(), "failed to create window, {}", SDL_GetError());
		Logger::info("created a {} window ({} x {})!", windowTitle, width, height);
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
		SystemEventManager::mWindowClose.addListener(mOnWindowClose);
	}

	Window::~Window()
	{
	}
}