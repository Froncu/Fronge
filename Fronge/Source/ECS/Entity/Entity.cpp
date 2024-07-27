#include "froch.hpp"

#include "Entity.hpp"
#include "Logger/Logger.hpp"

namespace fro
{
	std::unordered_set<Reference<Entity>> const& Entity::getAllEntities()
	{
		return sEntities;
	}

	std::unordered_set<Reference<Entity>> Entity::sEntities{};
	IDGenerator Entity::sIDGenerator{};

	Entity::Entity()
	{
		sEntities.insert(this);

		Logger::info("created Entity with ID {}!",
			mID);
	}

	Entity::~Entity()
	{
		sEntities.erase(this);

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