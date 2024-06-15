#if not defined WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

struct SDL_Window;

namespace fro
{
	class Window::Implementation final
	{
	public:
		Implementation(Window::Data& windowData);

		~Implementation() = default;

	private:
		static bool sSDLVideoInitialized;

		Implementation(Implementation const&) = delete;
		Implementation(Implementation&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&) noexcept = delete;

		SDL_Window* mSDLWindow;
	};
}

#endif