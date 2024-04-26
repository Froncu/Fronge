#pragma once

#include "Singleton.hpp"
#include "Event.hpp"

typedef union SDL_Event SDL_Event;

namespace fro
{
	class SystemEventManager final : public Singleton<SystemEventManager>
	{
	public:
		SystemEventManager();

		virtual ~SystemEventManager() override;

		[[nodiscard("eventual SDL_QUIT event ignored")]] bool processSystemEvents() const;

		Event<const SDL_Event&> m_SystemEvent{};

	private:
		SystemEventManager(const SystemEventManager&) = delete;
		SystemEventManager(SystemEventManager&&) noexcept = delete;

		SystemEventManager& operator=(const SystemEventManager&) = delete;
		SystemEventManager& operator=(SystemEventManager&&) noexcept = delete;
	};
}