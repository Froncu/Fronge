#if not defined SYSTEM_EVENT_MANAGER_HPP
#define SYSTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "EventSystem/EventSystem.hpp"

namespace fro
{
	class SystemEventManager final
	{
	public:
		FRO_API static EventDispatcher<std::uint32_t> mOnWindowClose;

		FRO_NODISCARD FRO_API static void pollEvents();

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