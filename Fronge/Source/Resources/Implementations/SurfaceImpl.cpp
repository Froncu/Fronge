#include "froch.hpp"

#include "SurfaceImpl.hpp"
#include "Renderer/Implementation/RendererImpl.hpp"
#include "Resources/Implementations/FontImpl.hpp"

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

	IDGenerator Surface::sIDGenerator{};

	Surface::Surface(std::string_view const imagePath)
		: mImplementation{ std::make_unique<Implementation>(imagePath) }
		, mSize{ mImplementation->getSDLSurface()->w, mImplementation->getSDLSurface()->h }
	{
		Logger::info("loaded {} as surface with ID {}!",
			imagePath, mID);
	}

	Surface::Surface(Font const& font, std::string_view const text)
		: mImplementation{ std::make_unique<Implementation>(font, text) }
		, mSize{ mImplementation->getSDLSurface()->w, mImplementation->getSDLSurface()->h }
	{
		Logger::info("loaded \"{}\" as Surface with ID {}!",
			text, mID);
	}

	Surface::Surface(Surface&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
		, mImplementation{ std::move(other.mImplementation) }
		, mSize{ other.getSize() }
	{
		other.mSize = {};
	}

	Surface::~Surface()
	{
		Logger::info("destroyed Surface with ID {}!",
			mID);
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);
		mImplementation = std::move(other.mImplementation);
		mSize = other.getSize();

		other.mSize = {};

		return *this;
	}

	Surface::Implementation& Surface::getImplementation() const
	{
		return *mImplementation;
	}

	ID const& Surface::getID() const
	{
		return mID;
	}

	Vector2<int> Surface::getSize() const
	{
		return mSize;
	}
}