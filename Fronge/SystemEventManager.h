#pragma once

#include "Singleton.hpp"
#include "Event.hpp"

typedef union SDL_Event SDL_Event;

namespace fro
{
	class SystemEventManager final : public Singleton<SystemEventManager>
	{
		fro_GENERATED_SINGLETON_BODY(SystemEventManager)

	public:
		[[nodiscard("eventual SDL_QUIT event ignored")]] bool processSystemEvents() const;

		Event<const SDL_Event&> m_SystemEvent{};
	};
}