#include "froch.hpp"

#include "TextureImpl.hpp"
#include "Renderer/Implementation/RendererImpl.hpp"
#include "Renderer/Renderer.hpp"
#include "Resources/Implementations/SurfaceImpl.hpp"
#include "Resources/Surface.hpp"

#include <SDL.h>

namespace fro
{
	Texture::Implementation::Implementation(Renderer& renderer, Surface const& surface)
		: mSDLTexture{ SDL_CreateTextureFromSurface(
			renderer.getImplementation().getSDLRenderer(), surface.getImplementation().getSDLSurface()),
			SDL_DestroyTexture }
	{
		if (not mSDLTexture.get())
			FRO_EXCEPTION("failed to upload Surface with ID {} as SDL_Texture to Renderer with ID {}!",
				surface.getID(), renderer.getID());
	}

	SDL_Texture* Texture::Implementation::getSDLTexture() const
	{
		return mSDLTexture.get();
	}

	IDGenerator Texture::sIDGenerator{};

	Texture::Texture(Renderer& renderer, Surface const& surface)
		: mRenderer{ renderer }
		, mSize{ surface.getSize() }
		, mImplementation{ std::make_unique<Implementation>(*mRenderer, surface) }
	{
		Logger::info("uploaded Surface with ID {} as Texture with ID {} to Renderer with ID {}!",
			surface.getID(), mID, renderer.getID());
	}

	Texture::Texture(Texture&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
		, mRenderer{ std::move(other.mRenderer) }
		, mSize{ other.getSize() }
		, mImplementation{ std::move(other.mImplementation)}
	{
		other.mSize = {};
	}

	Texture::~Texture()
	{
		Logger::info("destroyed Texture with ID {}!",
			mID);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);
		mRenderer = std::move(other.mRenderer);
		mSize = other.getSize();
		mImplementation = std::move(other.mImplementation);

		other.mSize = {};

		return *this;
	}

	std::size_t Texture::getID() const
	{
		return mID;
	}

	Reference<Renderer> Texture::getRenderer() const
	{
		return mRenderer;
	}

	Texture::Implementation & Texture::getImplementation() const
	{
		return *mImplementation;
	}

	Vector2<int> Texture::getSize() const
	{
		return mSize;
	}
}