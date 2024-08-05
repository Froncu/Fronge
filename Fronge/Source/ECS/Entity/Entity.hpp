#if not defined ENTITY_HPP
#define ENTITY_HPP

#include "ComponentSparseSet.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Idenitifer/IDGenerator.hpp"
#include "Reference/Reference.hpp"
#include "Utility/SparseSet.hpp"

namespace fro
{
	class Entity final : public Referencable
	{
	public:
		struct Global
		{

		};

		using DetachedComponent = std::pair<std::unique_ptr<Component>, std::type_index>;

		FRO_API FRO_NODISCARD static std::unordered_set<Reference<Entity>> const& getAllEntities();

		FRO_API static EventDispatcher<Entity, Component, std::type_index const> sComponentAttachEvent;
		FRO_API static EventDispatcher<Entity, Component, std::type_index const> sComponentDetachEvent;

		FRO_API Entity();
		FRO_API Entity(Entity&& other) noexcept;

		FRO_API ~Entity();

		FRO_API Entity& operator=(Entity&& other) noexcept;

		FRO_API FRO_NODISCARD ID const& getID() const;
		FRO_API std::vector<DetachedComponent> detachAll();

		template<ComponentSparseSetStorable ComponentType>
			requires std::default_initializable<ComponentType>
		FRO_NODISCARD ComponentType* getComponent()
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return nullptr;

			auto&& [component, inserted] { getSparseSet<ComponentType>()[ID] };
			if (inserted)
				sComponentAttachEvent.notify(*this, component, typeid(ComponentType));

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
				getSparseSet<ComponentType>().insert(ID, std::forward<ArgumentTypes>(arguments)...) };

			if (not attachedComponent)
				return attachedComponent;

			sComponentAttachEvent.notify(*this, *attachedComponent, typeid(ComponentType));

			return attachedComponent;
		}

		template<ComponentSparseSetStorable ComponentType>
		std::optional<ComponentType> detachComponent()
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return {};

			std::optional<ComponentType> detachedComponent{
				getSparseSet<ComponentType>().erase(ID) };

			if (not detachedComponent.has_value())
				return detachedComponent;

			sComponentDetachEvent.notify(*this, *detachedComponent, typeid(ComponentType));

			return detachedComponent;
		}

		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD ComponentType* findComponent() const
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return nullptr;

			return getSparseSet<ComponentType>().find(ID);
		}

		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD bool hasComponentAttached() const
		{
			std::size_t const ID{ getID() };
			if (ID == ID::INVALID_ID)
				return false;

			return getSparseSet<ComponentType>().contains(ID);
		}

	private:
		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD static SparseSet<ComponentType>& getSparseSet()
		{
			using ComponentSparseSetType = ComponentSparseSet<ComponentType>;

			auto& baseComponentSparseSet{ sBaseComponentSparseSets.emplace(typeid(ComponentType), std::make_unique<ComponentSparseSetType>()).first->second };
			return static_cast<ComponentSparseSetType&>(*baseComponentSparseSet).mSparseSet;
		}

		static IDGenerator sIDGenerator;
		static std::unordered_set<Reference<Entity>> sEntities;
		FRO_API static std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> sBaseComponentSparseSets;

		Entity(Entity const&) = delete;

		Entity& operator=(Entity const&) = delete;

		ID mID{ sIDGenerator.get() };
	};
}

template<>
struct std::hash<fro::Reference<fro::Entity>>
{
	std::size_t operator()(fro::Reference<fro::Entity> const& entity) const
	{
		return std::hash<std::size_t>{}(entity->getID());
	}
};

#endif