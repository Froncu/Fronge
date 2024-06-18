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
		FRO_API static Event<BaseEvent&> mSystem;
		FRO_API static Event<WindowEvent&> mWindow;
		FRO_API static Event<WindowCloseEvent&> mWindowClose;

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