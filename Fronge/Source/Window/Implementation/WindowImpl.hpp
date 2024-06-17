#if not defined WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <functional>
#include <memory>

struct SDL_Window;

namespace fro
{
	class Window::Implementation final
	{
	public:
		Implementation(Window::Data& windowData);

		~Implementation() = default;

		std::uint32_t getID() const;

	private:
		static bool sSDLVideoInitialized;

		Implementation(Implementation const&) = delete;
		Implementation(Implementation&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&) noexcept = delete;

		std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> mSDLWindow;
	};
}

#endif