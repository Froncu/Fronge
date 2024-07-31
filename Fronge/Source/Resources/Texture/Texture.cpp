#include "froch.hpp"

#include "Implementation/TextureImpl.hpp"
#include "Renderer/Renderer.hpp"
#include "Resources/Surface/Surface.hpp"
#include "Texture.hpp"

namespace fro
{
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
		, mImplementation{ std::move(other.mImplementation) }
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

	Texture::Implementation& Texture::getImplementation() const
	{
		return *mImplementation;
	}

	ID const& Texture::getID() const
	{
		return mID;
	}

	Reference<Renderer> Texture::getRenderer() const
	{
		return mRenderer;
	}

	Vector2<int> Texture::getSize() const
	{
		return mSize;
	}
}