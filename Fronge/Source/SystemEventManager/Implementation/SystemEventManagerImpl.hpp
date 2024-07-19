#if not defined SYSTEM_EVENT_MANAGER_IMPL_HPP
#define SYSTEM_EVENT_MANAGER_IMPL_HPP

#include "SystemEventManager/SystemEventManager.hpp"

struct SDL_ControllerAxisEvent;
struct SDL_ControllerButtonEvent;
struct SDL_ControllerDeviceEvent;
struct SDL_KeyboardEvent;
struct SDL_WindowEvent;

namespace fro
{
	class SystemEventManager::Implementation final
	{
	public:
		static void dispatchSDLWindowEvent(SDL_WindowEvent const& SDLEvent);
		static void dispatchSDLKeyboardEvent(SDL_KeyboardEvent const& SDLEvent);
		static void dispatchSDLControllerDeviceEvent(SDL_ControllerDeviceEvent const& SDLEvent);
		static void dispatchSDLControllerButtonEvent(SDL_ControllerButtonEvent const& SDLEvent);
		static void dispatchSDLControllerAxisEvent(SDL_ControllerAxisEvent const& SDLEvent);

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