#include "froch.hpp"

#include "Entity.hpp"
#include "Logger/Logger.hpp"

namespace fro
{
	std::unordered_set<Reference<Entity>> const& Entity::getAllEntities()
	{
		return getAllEntitiesInternal();
	}

	std::unordered_set<Reference<Entity>>& Entity::getAllEntitiesInternal()
	{
		static std::unordered_set<Reference<Entity>> entities{};
		return entities;
	}

	EventDispatcher<Entity, Component, std::type_index const>& Entity::getComponentAttachEvent()
	{
		static EventDispatcher<Entity, Component, std::type_index const> sComponentAttachEvent{};
		return sComponentAttachEvent;
	}

	EventDispatcher<Entity, Component, std::type_index const>& Entity::getComponentDetachEvent()
	{
		static EventDispatcher<Entity, Component, std::type_index const> sComponentDetachEvent{};
		return sComponentDetachEvent;
	}

	IDGenerator Entity::sIDGenerator{};
	std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> Entity::sBaseComponentSparseSets{};

	Entity::Entity()
	{
		getAllEntitiesInternal().insert(this);

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

		getAllEntitiesInternal().erase(this);

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

		for (auto&& [componentTypeIndex, baseComponentSparseSet] : sBaseComponentSparseSets)
		{
			std::unique_ptr<Component> component{ baseComponentSparseSet->detach(ID) };
			if (not component.get())
				continue;

			auto&& [detachedComponent, detachedComponentTypeIndex] { detachedComponents.emplace_back(std::move(component), componentTypeIndex) };
			getComponentDetachEvent().notify(*this, *detachedComponent, detachedComponentTypeIndex);
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