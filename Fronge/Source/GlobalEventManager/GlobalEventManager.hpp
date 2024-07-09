#if not defined SYSTEM_EVENT_MANAGER_HPP
#define SYSTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "Events/WindowEvent.hpp"

namespace fro
{
	class GlobalEventManager final
	{
	public:
		FRO_API static EventDispatcher<WindowCloseEvent&> mWindowCloseEvent;
		FRO_API static EventDispatcher<WindowResizeEvent&> mWindowResizeEvent;

		FRO_API FRO_NODISCARD static void pollEvents();

	private:
		GlobalEventManager() = delete;
		GlobalEventManager(GlobalEventManager const&) = delete;
		GlobalEventManager(GlobalEventManager&&) noexcept = delete;

		~GlobalEventManager() = delete;

		GlobalEventManager& operator=(GlobalEventManager const&) = delete;
		GlobalEventManager& operator=(GlobalEventManager&&) noexcept = delete;
	};
}

#endif