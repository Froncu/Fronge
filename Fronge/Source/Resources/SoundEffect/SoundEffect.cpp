#include "froch.hpp"

#include "Implementation/SoundEffectImpl.hpp"
#include "Logger/Logger.hpp"
#include "SoundEffect.hpp"

namespace fro
{
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

	ID const& SoundEffect::getID() const
	{
		return mID;
	}

	int SoundEffect::getChannel() const
	{
		return mChannel;
	}
}