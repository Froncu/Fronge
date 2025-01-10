#ifndef SYTEM_EVENT_MANAGER_HPP
#define SYTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "Events/InputEvent.hpp"
#include "Events/Systems/EventListener.hpp" 
#include "Events/WindowEvent.hpp"

namespace fro
{
	class SystemEventManager final
	{
	public:
		class Implementation;

		FRO_API static void initialize();
		FRO_API static void shutDown();

		FRO_API FRO_NODISCARD static void pollEvents();

		FRO_API static EventDispatcher<WindowEvent const> mWindowEvent;
		FRO_API static EventDispatcher<InputEvent const> mInputEvent;

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