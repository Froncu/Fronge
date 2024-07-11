#include "froch.hpp"

#include "TextureImpl.hpp"
#include "Renderer/Implementation/RendererImpl.hpp"

#include <SDL.h>
#include <SDL_image.h>

namespace fro
{
	Texture::Implementation::Implementation(Reference<Renderer> const renderer, std::string_view const imagePath)
		: mSDLTexture{ createTexture(renderer, imagePath) }
	{
	}

	SDL_Texture* Texture::Implementation::getSDLTexture() const
	{
		return mSDLTexture.get();
	}

	CustomUniquePointer<SDL_Texture> Texture::Implementation::createTexture(
		Reference<Renderer> const renderer, std::string_view const imagePath)
	{
		CustomUniquePointer<SDL_Surface> const surface{
			IMG_Load(imagePath.data()), SDL_FreeSurface };

		if (not surface.get())
			FRO_EXCEPTION("failed to load {} as SDL_Surface ({})", imagePath, IMG_GetError());

		CustomUniquePointer<SDL_Texture> texture{
			SDL_CreateTextureFromSurface(renderer->getImplementation().getSDLRenderer(), surface.get()), SDL_DestroyTexture };

		if (not texture.get())
			FRO_EXCEPTION("failed to load {} as SDL_Texture from SDL_Surface ({})", imagePath, SDL_GetError());

		return texture;
	}

	IDGenerator Texture::sIDGenerator{};

	Texture::Texture(Reference<Renderer> const renderer, std::string_view const imagePath)
		: mRenderer{ renderer }
		, mImplementation{ std::make_unique<Implementation>(renderer, imagePath) }
	{
		SDL_QueryTexture(mImplementation->getSDLTexture(), nullptr, nullptr, &mWidth, &mHeight);

		Logger::info("{} loaded as texture with ID {}!",
			imagePath, mID);
	}

	Texture::Texture(Texture&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
		, mRenderer{ std::move(other.mRenderer) }
		, mImplementation{ std::move(other.mImplementation) }
		, mWidth{ other.getWidth() }
		, mHeight{ other.getHeight() }
	{
		other.mWidth = 0;
		other.mHeight = 0;
	}

	Texture::~Texture()
	{
		Logger::info("texture with ID {} destroyed!",
			mID);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);
		mRenderer = std::move(other.mRenderer);
		mImplementation = std::move(other.mImplementation);
		mWidth = other.getWidth();
		mHeight = other.getHeight();

		other.mWidth = 0;
		other.mHeight = 0;

		return *this;
	}

	Texture::Implementation& Texture::getImplementation() const
	{
		return *mImplementation;
	}

	std::size_t Texture::getID() const
	{
		return mID;
	}

	int Texture::getWidth() const
	{
		return mWidth;
	}

	int Texture::getHeight() const
	{
		return mHeight;
	}
}