#include "Logger.hpp"
#include "Window/Window.hpp"
#include "WindowImpl.hpp"

namespace fro
{
	bool Window::Implementation::sSDLVideoInitialized{};

	Window::Implementation::Implementation(Data& windowData)
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
		mSDLWindow = SDL_CreateWindow(windowData.title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			windowData.width, windowData.height,
			NULL);

		FRO_ASSERT(mSDLWindow, "failed to create window, {}", SDL_GetError());
		Logger::info("created a {} window ({} x {})!", windowData.title, windowData.width, windowData.height);

		SDL_SetWindowData(mSDLWindow, windowData.title.c_str(), static_cast<void*>(&windowData));
	}

	Window::Window(std::string title, int const width, int const height)
		: mData
		{
			.title{ std::move(title) },
			.width{ width },
			.height{ height }
		}
		, mImplementation{ std::make_unique<Implementation>(mData) }
	{
	}

	Window::~Window()
	{
	}

	void Window::update()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event));
	}
}