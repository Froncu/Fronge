#pragma once

#include "Singleton.hpp"

typedef union SDL_Event SDL_Event;

namespace fro
{
	class GUIContext final : public Singleton<GUIContext>
	{
		fro_GENERATED_SINGLETON_BODY(GUIContext)

	public:
		void processSystemEvent(const SDL_Event& event) const;
		void startFrame() const;
		void endFrame() const;
	};
}