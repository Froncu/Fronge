#include "froch.hpp"

#include "Core.hpp"
#include "Logger/Logger.hpp"
#include "SoundEffectImpl.hpp"

#include <SDL_mixer.h>

namespace fro
{
	SoundEffect::Implementation::Implementation(std::string_view const filePath)
		: mSDLSoundEffect{ Mix_LoadWAV(filePath.data()), Mix_FreeChunk }
	{
		if (not mSDLSoundEffect.get())
			FRO_EXCEPTION("failed to load sound effect ({})", Mix_GetError());

		Logger::info("{} loaded as sound effect!", filePath);
	}

	Mix_Chunk* SoundEffect::Implementation::getSDLSoundEffect() const
	{
		return mSDLSoundEffect.get();
	}

	SoundEffect::SoundEffect(Descriptor descriptor)
		: mDescriptor{ std::move(descriptor) }
		, mImplementation{ std::make_unique<Implementation>(mDescriptor.filePath) }
	{
	}

	SoundEffect::SoundEffect(SoundEffect const& other)
		: Referencable(other)

		, mDescriptor{ other.mDescriptor }
		, mImplementation{ std::make_unique<Implementation>(mDescriptor.filePath) }
	{
	}

	SoundEffect::SoundEffect(SoundEffect&& other) noexcept
		: Referencable(std::move(other))

		, mDescriptor{ std::move(other.mDescriptor) }
		, mImplementation{ std::move(other.mImplementation) }
	{
	}

	SoundEffect::~SoundEffect()
	{
	}

	SoundEffect& SoundEffect::operator=(SoundEffect const& other)
	{
		if (this == &other)
			return *this;

		Referencable::operator=(other);

		mDescriptor = other.mDescriptor;
		mImplementation = std::make_unique<Implementation>(mDescriptor.filePath);

		return *this;
	}

	SoundEffect& SoundEffect::operator=(SoundEffect&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mDescriptor = std::move(other.mDescriptor);
		mImplementation = std::move(other.mImplementation);

		return *this;
	}

	SoundEffect::Implementation& SoundEffect::getImplementation() const
	{
		return *mImplementation;
	}

	int SoundEffect::getChannel() const
	{
		return mChannel;
	}

	std::string_view SoundEffect::getFilePath() const
	{
		return mDescriptor.filePath;
	}
}