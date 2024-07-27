#include "froch.hpp"

#include "Entity.hpp"
#include "Logger/Logger.hpp"

namespace fro
{
	std::size_t Entity::getHighestTakenID()
	{
		return sIDGenerator.highestTakenID();
	}

	IDGenerator Entity::sIDGenerator{};

	Entity::Entity()
	{
		Logger::info("created Entity with ID {}!",
			mID);
	}

	Entity::~Entity()
	{
		Logger::info("destroyed Entity with ID {}!",
			mID);
	}


	Entity::operator std::size_t() const
	{
		return getID();
	}

	ID const& Entity::getID() const
	{
		return mID;
	}
}