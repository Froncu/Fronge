#include "froch.hpp"

#include "SurfaceImpl.hpp"
#include "Renderer/Implementation/RendererImpl.hpp"
#include "Resources/Font/Implementation/FontImpl.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>

namespace fro
{
	Surface::Implementation::Implementation(std::string_view const imagePath)
		: mSDLSurface{ loadSurface(imagePath) }
	{
	}

	Surface::Implementation::Implementation(Font const& font, std::string_view const text)
		: mSDLSurface{ renderTextToSurface(font, text) }
	{
	}

	SDL_Surface* Surface::Implementation::getSDLSurface() const
	{
		return mSDLSurface.get();
	}

	CustomUniquePointer<SDL_Surface> Surface::Implementation::loadSurface(std::string_view const imagePath)
	{
		CustomUniquePointer<SDL_Surface> surface{ IMG_Load(imagePath.data()), SDL_FreeSurface };

		if (not surface.get())
			FRO_EXCEPTION("failed to load {} as SDL_Surface ({})",
				imagePath, IMG_GetError());

		return surface;
	}

	CustomUniquePointer<SDL_Surface> Surface::Implementation::renderTextToSurface(Font const& font, std::string_view const text)
	{
		CustomUniquePointer<SDL_Surface> surface{
			TTF_RenderText_Blended(font.getImplementation().getSDLFont(), text.data(), SDL_Color(255, 255, 255, 255)),
			SDL_FreeSurface };

		if (not surface.get())
			FRO_EXCEPTION("failed to render \"{}\" with Font ID {} as SDL_Surface ({})",
				text, font.getID(), TTF_GetError());

		return surface;
	}
}