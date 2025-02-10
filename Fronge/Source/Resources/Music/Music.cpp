#include "froch.hpp"

#include "Implementation/MusicImpl.hpp"
#include "Logger/Logger.hpp"
#include "Music.hpp"

namespace fro
{
	IDGenerator Music::sIDGenerator{};

	Music::Music(std::string_view const filePath)
		: mImplementation{ std::make_unique<Implementation>(filePath) }
	{
		Logger::info("loaded {} as Music with ID {}!",
			filePath, mID);
	}

	Music::Music(Music&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
		, mImplementation{ std::move(other.mImplementation) }
	{
	}

	Music::~Music()
	{
		Logger::info("destroyed Music with ID {}!",
			mID);
	}

	Music& Music::operator=(Music&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);
		mImplementation = std::move(other.mImplementation);

		return *this;
	}

	Music::Implementation& Music::getImplementation() const
	{
		return *mImplementation;
	}

	ID const& Music::getID() const
	{
		return mID;
	}
}