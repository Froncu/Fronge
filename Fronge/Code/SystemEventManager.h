#if not defined froSYSTEM_EVENT_MANAGERT_H
#define fro_SYSTEM_EVENT_MANAGER_H

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

		Event<SDL_Event const&> m_SystemEvent{};

	private:
		SystemEventManager(SystemEventManager const&) = delete;
		SystemEventManager(SystemEventManager&&) noexcept = delete;

		SystemEventManager& operator=(SystemEventManager const&) = delete;
		SystemEventManager& operator=(SystemEventManager&&) noexcept = delete;
	};
}

#endif