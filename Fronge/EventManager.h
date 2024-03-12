#pragma once

namespace fro
{
	class EventManager
	{
	public:
		static [[nodiscard("eventual SDL_QUIT event ignored")]] bool processEvents();

	private:
		EventManager() = delete;
		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) noexcept = delete;

		~EventManager() = delete;

		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(EventManager&&) noexcept = delete;
	};
}