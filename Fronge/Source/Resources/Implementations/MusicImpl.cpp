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
			FRO_EXCEPTION("failed to load {} as Mix_Music ({})",
				filePath, Mix_GetError());
	}

	Mix_Music* Music::Implementation::getSDLMusic() const
	{
		return mSDLMusic.get();
	}

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

	std::size_t Music::getID() const
	{
		return mID;
	}
}