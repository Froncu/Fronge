#if not defined SYSTEM_EVENT_MANAGER_IMPL
#define SYSTEM_EVENT_MANAGER_IMPL

#include "SystemEventManager/SystemEventManager.hpp"

struct SDL_KeyboardEvent;
struct SDL_WindowEvent;

namespace fro
{
	class SystemEventManager::Implementation final
	{
	public:
		static void dispatchSDLWindowEvent(SDL_WindowEvent const& SDLWindowEvent);
		static void dispatchSDLKeyboardEvent(SDL_KeyboardEvent const& SDLKeyboardEvent);

	private:
		Implementation() = delete;
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		~Implementation() = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;
	};
}

#endif