#include "froch.hpp"

#include "Scene.hpp"

namespace fro
{
	void fro::Scene::doomEntities()
	{
		auto const newEnd
		{
			std::remove_if(mEntities.begin(), mEntities.end(),
				[](Entity& entity)
				{
					return entity.isDoomed();
				})
		};

		mEntities.erase(newEnd, mEntities.end());
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
		return storedEntity;
	}
}