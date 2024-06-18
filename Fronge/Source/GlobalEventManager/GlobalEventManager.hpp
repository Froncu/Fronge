#if not defined SYSTEM_EVENT_MANAGER_HPP
#define SYSTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "Events/Events.hpp"
#include "Events/Polymorphic/WindowCloseEvent.hpp"

namespace fro
{
	class GlobalEventManager final
	{
	public:
		FRO_API static EventDispatcher<Event&> mSystemEvent;
		FRO_API static EventDispatcher<WindowEvent&> mWindowEvent;
		FRO_API static EventDispatcher<WindowCloseEvent&> mWindowCloseEvent;

		FRO_NODISCARD FRO_API static void pollEvents();

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