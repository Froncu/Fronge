#pragma once

#include "Singleton.hpp"

struct SDL_Window;
struct SDL_Renderer;
typedef union SDL_Event SDL_Event;

namespace fro
{
	class GUI final : public Singleton<GUI>
	{
		fro_GENERATED_SINGLETON_BODY(GUI)

	public:
		void processEvent(const SDL_Event& event) const;
		void startFrame() const;
		void endFrame() const;
	};
}