#include "froch.hpp"

#include "Audio/Audio.hpp"
#include "Core.hpp"
#include "Logger/Logger.hpp"
#include "MusicImpl.hpp"

#include <SDL_mixer.h>

namespace fro
{
	Music::Implementation::Implementation(std::string_view const filePath)
		: mSDLMusic{ Mix_LoadMUS(filePath.data()), Mix_FreeMusic }
	{
		if (not mSDLMusic.get())
			FRO_EXCEPTION("failed to load music ({})", Mix_GetError());

		Logger::info("{} loaded as music!", filePath);
	}

	Mix_Music* Music::Implementation::getSDLMusic() const
	{
		return mSDLMusic.get();
	}

	Music::Music(Descriptor descriptor)
		: mDescriptor{ std::move(descriptor) }
		, mImplementation{ std::make_unique<Implementation>(mDescriptor.filePath) }
	{
	}

	Music::Music(Music const& other)
		: Referencable(other)

		, mDescriptor{ other.mDescriptor }
		, mImplementation{ std::make_unique<Implementation>(mDescriptor.filePath) }
	{
	}

	Music::Music(Music&& other) noexcept
		: Referencable(std::move(other))

		, mDescriptor{ std::move(other.mDescriptor) }
		, mImplementation{ std::move(other.mImplementation) }
	{
	}

	Music::~Music()
	{
	}

	Music& Music::operator=(Music const& other)
	{
		if (this == &other)
			return *this;

		Referencable::operator=(other);

		mDescriptor = other.mDescriptor;
		mImplementation = std::make_unique<Implementation>(mDescriptor.filePath);

		return *this;
	}

	Music& Music::operator=(Music&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mDescriptor = std::move(other.mDescriptor);
		mImplementation = std::move(other.mImplementation);

		return *this;
	}

	Music::Implementation& Music::getImplementation() const
	{
		return *mImplementation;
	}

	std::string_view Music::getFilePath() const
	{
		return mDescriptor.filePath;
	}
}