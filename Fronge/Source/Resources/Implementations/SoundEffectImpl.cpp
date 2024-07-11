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
			FRO_EXCEPTION("failed to load {} as Mix_Chunk ({})",
				filePath, Mix_GetError());
	}

	Mix_Chunk* SoundEffect::Implementation::getSDLSoundEffect() const
	{
		return mSDLSoundEffect.get();
	}

	IDGenerator SoundEffect::sIDGenerator{};

	SoundEffect::SoundEffect(std::string_view const filePath)
		: mImplementation{ std::make_unique<Implementation>(filePath) }
	{
		Logger::info("loaded {} as SoundEffect with ID {}!",
			filePath, mID);
	}

	SoundEffect::SoundEffect(SoundEffect&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
		, mImplementation{ std::move(other.mImplementation) }
		, mChannel{ other.getChannel() }
	{
		other.mChannel = -1;
	}

	SoundEffect::~SoundEffect()
	{
		Logger::info("destroyed SoundEffect with ID {}!",
			mID);
	}

	SoundEffect& SoundEffect::operator=(SoundEffect&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);
		mImplementation = std::move(other.mImplementation);
		mChannel = other.getChannel();

		other.mChannel = -1;

		return *this;
	}

	SoundEffect::Implementation& SoundEffect::getImplementation() const
	{
		return *mImplementation;
	}

	std::size_t SoundEffect::getID() const
	{
		return mID;
	}

	int SoundEffect::getChannel() const
	{
		return mChannel;
	}
}