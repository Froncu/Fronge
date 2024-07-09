#if not defined WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include "froch.hpp"

#include "Utility.hpp"

struct SDL_Window;

namespace fro
{
	class Window::Implementation final
	{
	public:
		Implementation(std::string_view const windowTitle, int const width, int const height);

		~Implementation() = default;

		SDL_Window* get() const;

		std::string_view getTitle() const;
		std::uint32_t getID() const;

	private:
		static bool sSDLVideoInitialized;

		Implementation(Implementation const&) = delete;
		Implementation(Implementation&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&) noexcept = delete;

		CustomUniquePointer<SDL_Window> mSDLWindow;
	};
}

#endif