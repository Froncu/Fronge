#include "froch.hpp"

#include "Entity.hpp"
#include "Logger/Logger.hpp"

namespace fro
{
	std::unordered_set<Reference<Entity>> const& Entity::getAllEntities()
	{
		return sEntities;
	}

	EventDispatcher<Entity, Component, std::type_index const> Entity::sComponentAttachEvent{};
	EventDispatcher<Entity, Component, std::type_index const> Entity::sComponentDetachEvent{};
	IDGenerator Entity::sIDGenerator{};
	std::unordered_set<Reference<Entity>> Entity::sEntities{};
	std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> Entity::sBaseComponentSparseSets{};

	Entity::Entity()
	{
		sEntities.insert(this);

		Logger::info("created Entity with ID {}!",
			mID);
	}

	Entity::Entity(Entity&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
	{
	}

	Entity::~Entity()
	{
		detachAll();

		sEntities.erase(this);

		Logger::info("destroyed Entity with ID {}!",
			mID);
	}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);

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

		for (auto&& [componentTypeIndex, baseComponentSparseSet] : sBaseComponentSparseSets)
		{
			std::unique_ptr<Component> component{ baseComponentSparseSet->detach(ID) };
			if (not component.get())
				continue;

			auto&& [detachedComponent, detachedComponentTypeIndex] { detachedComponents.emplace_back(std::move(component), componentTypeIndex) };
			sComponentDetachEvent.notify(*this, *detachedComponent, detachedComponentTypeIndex);
		}

		return detachedComponents;
	}
}