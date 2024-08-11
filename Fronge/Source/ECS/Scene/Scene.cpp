#include "froch.hpp"

#include "Scene.hpp"

namespace fro
{
	void Scene::doomAndAdd()
	{
		auto const newEnd
		{
			std::remove_if(mEntities.begin(), mEntities.end(),
				[](Entity const& entity)
				{
					return entity.isDoomed();
				})
		};

		mEntities.erase(newEnd, mEntities.end());

		for (Entity& entity : mEntitiesToAdd)
		{
			Entity& addedEntity{ mEntities.emplace_back(std::move(entity)) };
			addedEntity.mParentingScene = this;
			EntityManager::getAddedToSceneEvent().notify(addedEntity, *this);
		}

		mEntitiesToAdd.clear();
	}

	std::vector<Entity> const& Scene::getEntities() const
	{
		return mEntities;
	}

	std::optional<Entity> Scene::removeEntity(Entity const& entity)
	{
		auto const foundEntity
		{
			std::find_if(mEntities.begin(), mEntities.end(),
			[&entity](Entity& storedEntity)
			{
				return &entity == &storedEntity;
			})
		};

		if (foundEntity == mEntities.end())
			return {};

		Entity storedEntity{ std::move(*foundEntity) };
		mEntities.erase(foundEntity);
		EntityManager::getRemovedFromSceneEvent().notify(storedEntity, *this);
		return storedEntity;
	}
}