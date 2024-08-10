#if not defined ENTITY_HPP
#define ENTITY_HPP

#include "EntityManager/EntityManager.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Idenitifer/IDGenerator.hpp"
#include "Reference/Reference.hpp"
#include "Utility/SparseSet.hpp"

namespace fro
{
	class Entity final : public Referencable
	{
	public:
		using DetachedComponent = std::pair<std::unique_ptr<Component>, std::type_index>;

		FRO_API Entity();
		FRO_API Entity(Entity&& other) noexcept;

		FRO_API ~Entity();

		FRO_API Entity& operator=(Entity&& other) noexcept;

		FRO_API FRO_NODISCARD ID const& getID() const;
		FRO_API std::vector<DetachedComponent> detachAll();
		FRO_API void markDoomed();
		FRO_API FRO_NODISCARD bool isDoomed() const;

		template<ComponentSparseSetStorable ComponentType>
			requires std::default_initializable<ComponentType>
		FRO_NODISCARD ComponentType* getComponent()
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return nullptr;

			auto&& [component, inserted] { EntityManager::getSparseSet<ComponentType>()[ID] };
			if (inserted)
				EntityManager::getComponentAttachEvent().notify(*this, component, typeid(ComponentType));

			return &component;
		}

		template<ComponentSparseSetStorable ComponentType, typename... ArgumentTypes>
			requires std::constructible_from<ComponentType, ArgumentTypes...>
		ComponentType* attachComponent(ArgumentTypes&&... arguments)
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return nullptr;

			ComponentType* const attachedComponent{
				EntityManager::getSparseSet<ComponentType>().insert(ID, std::forward<ArgumentTypes>(arguments)...) };

			if (not attachedComponent)
				return attachedComponent;

			EntityManager::getComponentAttachEvent().notify(*this, *attachedComponent, typeid(ComponentType));

			return attachedComponent;
		}

		template<ComponentSparseSetStorable ComponentType>
		std::optional<ComponentType> detachComponent()
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return {};

			std::optional<ComponentType> detachedComponent{
				EntityManager::getSparseSet<ComponentType>().erase(ID) };

			if (not detachedComponent.has_value())
				return detachedComponent;

			EntityManager::getComponentDetachEvent().notify(*this, *detachedComponent, typeid(ComponentType));

			return detachedComponent;
		}

		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD ComponentType* findComponent() const
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return nullptr;

			return EntityManager::getSparseSet<ComponentType>().find(ID);
		}

		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD bool hasComponentAttached() const
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return false;

			return EntityManager::getSparseSet<ComponentType>().contains(ID);
		}

	private:
		static IDGenerator sIDGenerator;

		Entity(Entity const&) = delete;

		Entity& operator=(Entity const&) = delete;

		ID mID{ sIDGenerator.get() };
		bool mIsDoomed{};
	};
}

template<>
struct std::hash<fro::Reference<fro::Entity>>
{
	std::size_t operator()(fro::Reference<fro::Entity> const& entity) const
	{
		if (not entity.valid())
			return fro::ID::INVALID_ID;

		return static_cast<std::size_t>(entity->getID());
	}
};

template<>
struct std::equal_to<fro::Reference<fro::Entity>>
{
	bool operator()(fro::Reference<fro::Entity> const& entity1, fro::Reference<fro::Entity> const& entity2) const
	{
		if (not entity1.valid() or not entity2.valid())
			return false;

		return &*entity1 == &*entity2;
	}
};

#endif