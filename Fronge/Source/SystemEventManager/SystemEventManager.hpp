#if not defined SYSTEM_EVENT_MANAGER_HPP
#define SYSTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "EventSystem/EventSystem.hpp"
#include "EventSystem/Events/WindowCloseEvent.hpp"

namespace fro
{
	class SystemEventManager final
	{
	public:
		FRO_API static void pollEvents();

		FRO_API static EventDispatcher<WindowCloseEvent&> mOnWindowClose;

	private:
		SystemEventManager() = delete;
		SystemEventManager(SystemEventManager const&) = delete;
		SystemEventManager(SystemEventManager&&) noexcept = delete;

		~SystemEventManager() = delete;

		SystemEventManager& operator=(SystemEventManager const&) = delete;
		SystemEventManager& operator=(SystemEventManager&&) noexcept = delete;
	};
}

#endif