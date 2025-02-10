#include "froch.hpp"

#include "TextureImpl.hpp"
#include "Renderer/Implementation/RendererImpl.hpp"
#include "Renderer/Renderer.hpp"
#include "Resources/Surface/Implementation/SurfaceImpl.hpp"
#include "Resources/Surface/Surface.hpp"

#include <SDL.h>

namespace fro
{
	Texture::Implementation::Implementation(Renderer& renderer, Surface const& surface)
		: mSDLTexture{ SDL_CreateTextureFromSurface(
			renderer.getImplementation().getSDLRenderer(), surface.getImplementation().getSDLSurface()),
			SDL_DestroyTexture }
	{
		if (not mSDLTexture.get())
			exception("failed to upload Surface with ID {} as SDL_Texture to Renderer with ID {}!",
				surface.getID(), renderer.getID());
	}

	SDL_Texture* Texture::Implementation::getSDLTexture() const
	{
		return mSDLTexture.get();
	}
}