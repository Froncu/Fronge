#include "froch.hpp"

#include "RendererImpl.hpp"
#include "Window/Window.hpp"
#include "Window/Implementation/WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	Renderer::Implementation::Implementation(Window const& window)
		: mSDLRenderer{ SDL_CreateRenderer(window.getImplementation().getSDLWindow(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer }
	{
		if (not mSDLRenderer.get())
			FRO_EXCEPTION("failed to create SDL_Renderer ({})", SDL_GetError());
	}

	SDL_Renderer* Renderer::Implementation::getSDLRenderer() const
	{
		return mSDLRenderer.get();
	}
}