#include "froch.hpp"

#include "TextureImpl.hpp"
#include "Renderer/Implementation/RendererImpl.hpp"

#include <SDL.h>
#include <SDL_image.h>

namespace fro
{
	Texture::Implementation::Implementation(Descriptor const& descriptor)
		: mSDLTexture{ createTexture(descriptor) }
	{
	}

	SDL_Texture* Texture::Implementation::getSDLTexture() const
	{
		return mSDLTexture.get();
	}

	CustomUniquePointer<SDL_Texture> Texture::Implementation::createTexture(Descriptor const& descritptor)
	{
		CustomUniquePointer<SDL_Surface> const surface{
			IMG_Load(descritptor.filePath.c_str()), SDL_FreeSurface };

		if (not surface.get())
			FRO_EXCEPTION("failed to load {} as SDL_Surface ({})", descritptor.filePath, IMG_GetError());

		CustomUniquePointer<SDL_Texture> texture{
			SDL_CreateTextureFromSurface(descritptor.renderer->getImplementation().getSDLRenderer(), surface.get()), SDL_DestroyTexture };

		if (not texture.get())
			FRO_EXCEPTION("failed to load {} as SDL_Texture from SDL_Surface ({})", descritptor.filePath, SDL_GetError());

		return texture;
	}

	Texture::Texture(Descriptor descriptor)
		: mDescriptor{ std::move(descriptor) }
		, mImplementation{ std::make_unique<Implementation>(mDescriptor) }
	{
		SDL_QueryTexture(mImplementation->getSDLTexture(), nullptr, nullptr, &mWidth, &mHeight);

		Logger::info("loaded {} as texture!", mDescriptor.filePath);
	}

	Texture::Texture(Texture const& other)
		: Referencable(other)

		, mDescriptor{ other.mDescriptor }
		, mImplementation{ std::make_unique<Implementation>(mDescriptor) }
		, mWidth{ other.getWidth() }
		, mHeight{ other.getHeight() }
	{
		Logger::info("loaded {} as texture!", mDescriptor.filePath);
	}

	Texture::Texture(Texture&& other) noexcept
		: Referencable(std::move(other))

		, mDescriptor{ std::move(other.mDescriptor) }
		, mImplementation{ std::move(other.mImplementation) }
		, mWidth{ other.getWidth() }
		, mHeight{ other.getHeight() }
	{
		other.mWidth = 0;
		other.mHeight = 0;

		Logger::info("loaded {} as texture!", mDescriptor.filePath);
	}

	Texture::~Texture()
	{
	}

	Texture& Texture::operator=(Texture const& other)
	{
		if (this == &other)
			return *this;

		Referencable::operator=(other);

		mDescriptor = other.mDescriptor;
		mImplementation = std::make_unique<Implementation>(mDescriptor);
		mWidth = other.getWidth();
		mHeight = other.getHeight();

		return *this;
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mDescriptor = std::move(other.mDescriptor);
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

	std::string_view Texture::getFilePath() const
	{
		return mDescriptor.filePath;
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