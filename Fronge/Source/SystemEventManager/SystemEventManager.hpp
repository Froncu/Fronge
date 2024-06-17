#if not defined SYSTEM_EVENT_MANAGER_HPP
#define SYSTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "Events/Events.hpp"

namespace fro
{
	class SystemEventManager final
	{
	public:
		FRO_API static Event<std::uint32_t> mWindowClose;

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