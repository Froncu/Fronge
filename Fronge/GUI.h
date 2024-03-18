#pragma once

struct SDL_Window;
struct SDL_Renderer;
typedef union SDL_Event SDL_Event;

namespace fro
{
	class GUI final
	{
	public:
		static void initialise(SDL_Window* const pWindow, SDL_Renderer* const pRenderer);
		static void destroy();

		static void processEvent(const SDL_Event& event);
		static void startFrame();
		static void endFrame();

	private:
		GUI() = delete;
		GUI(const GUI&) = delete;
		GUI(GUI&&) noexcept = delete;

		~GUI() = delete;

		GUI& operator=(const GUI&) = delete;
		GUI& operator=(GUI&&) noexcept = delete;
	};
}