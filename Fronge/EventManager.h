#pragma once

#include "Singleton.hpp"

namespace fro
{
	class EventManager final : public Singleton<EventManager>
	{
		fro_GENERATED_SINGLETON_BODY(EventManager)

	public:
		[[nodiscard("eventual SDL_QUIT event ignored")]] bool processEvents() const;
	};
}