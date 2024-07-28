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

	Entity::Entity(Entity&& other) noexcept
		: Referencable(std::move(other))
	{
	}

	Entity::~Entity()
	{
		sEntities.erase(this);

		Logger::info("destroyed Entity with ID {}!",
			mID);
	}


	Entity& Entity::operator=(Entity&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		return *this;
	}

	ID const& Entity::getID() const
	{
		return mID;
	}
}