#if not defined SYSTEM_EVENT_MANAGER_HPP
#define SYSTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "Events/WindowEvent.hpp"
#include "Events/Systems/EventListener.hpp"

namespace fro
{
	class GlobalEventManager final
	{
	public:
		FRO_API FRO_NODISCARD static void pollEvents();

		FRO_API static EventDispatcher<WindowEvent const> mWindowEvent;

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