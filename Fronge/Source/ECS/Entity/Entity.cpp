#include "froch.hpp"

#include "Entity.hpp"
#include "Logger/Logger.hpp"

namespace fro
{
	IDGenerator Entity::sIDGenerator{};

	Entity::Entity()
	{
		EntityManager::getAllEntitiesInternal().insert(this);

		Logger::info("created Entity with ID {}!",
			mID);
	}

	Entity::Entity(Entity&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
		, mIsDoomed{ other.mIsDoomed }
	{
	}

	Entity::~Entity()
	{
		detachAll();

		EntityManager::getAllEntitiesInternal().erase(this);

		Logger::info("destroyed Entity with ID {}!",
			mID);
	}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		if (this == &other)
			return *this;

		detachAll();

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);
		mIsDoomed = other.mIsDoomed;

		return *this;
	}

	ID const& Entity::getID() const
	{
		return mID;
	}

	std::vector<Entity::DetachedComponent> Entity::detachAll()
	{
		std::size_t const ID{ getID() };
		if (ID == ID::INVALID_ID)
			return {};

		std::vector<DetachedComponent> detachedComponents{};

		for (auto&& [componentTypeIndex, baseComponentSparseSet] : EntityManager::getBaseComponentSparseSets())
		{
			std::unique_ptr<Component> component{ baseComponentSparseSet->detach(ID) };
			if (not component.get())
				continue;

			auto&& [detachedComponent, detachedComponentTypeIndex] { detachedComponents.emplace_back(std::move(component), componentTypeIndex) };
			EntityManager::getComponentDetachEvent().notify(*this, *detachedComponent, detachedComponentTypeIndex);
		}

		return detachedComponents;
	}

	void Entity::markDoomed()
	{
		if (mIsDoomed)
			return;

		mIsDoomed = true;
	}

	bool Entity::isDoomed() const
	{
		return mIsDoomed;
	}
}