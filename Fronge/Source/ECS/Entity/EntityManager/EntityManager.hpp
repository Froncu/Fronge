#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "ComponentSparseSet.hpp"
#include "Core.hpp"
#include "Events/Systems/EventDispatcher.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class Component;
	class Entity;
	class Scene;

	class EntityManager final
	{
		friend Entity;

	public:
		FRO_API FRO_NODISCARD static std::unordered_set<Reference<Entity>> const& getAllEntities();
		FRO_API FRO_NODISCARD static EventDispatcher<Entity, Component, std::type_index const>& getComponentAttachEvent();
		FRO_API FRO_NODISCARD static EventDispatcher<Entity, Component, std::type_index const>& getComponentDetachEvent();
		FRO_API FRO_NODISCARD static EventDispatcher<Entity, Scene>& getAddedToSceneEvent();
		FRO_API FRO_NODISCARD static EventDispatcher<Entity, Scene>& getRemovedFromSceneEvent();

	private:
		FRO_API FRO_NODISCARD static std::unordered_set<Reference<Entity>>& getAllEntitiesInternal();
		FRO_API FRO_NODISCARD static std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>>& getBaseComponentSparseSets();

		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD static SparseSet<ComponentType>& getSparseSet()
		{
			using ComponentSparseSetType = ComponentSparseSet<ComponentType>;

			std::unique_ptr<ComponentSparseSetType> temporary{};
			auto& baseComponentSparseSet{ getBaseComponentSparseSets().emplace(typeid(ComponentType), std::move(temporary)).first->second};
			if (not baseComponentSparseSet.get())
				baseComponentSparseSet = std::make_unique<ComponentSparseSetType>();

			return static_cast<ComponentSparseSetType&>(*baseComponentSparseSet).mSparseSet;
		}

		EntityManager() = delete;
		EntityManager(EntityManager const&) = delete;
		EntityManager(EntityManager&&) noexcept = delete;

		~EntityManager() = delete;

		EntityManager& operator=(EntityManager const&) = delete;
		EntityManager& operator=(EntityManager&&) noexcept = delete;
	};
}

#endif