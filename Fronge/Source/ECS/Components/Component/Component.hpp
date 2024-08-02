#if not defined COMPONENT_HPP
#define COMPONENT_HPP

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class Entity;

	class Component : public Referencable
	{
	public:
		virtual ~Component() override = default;

		FRO_API FRO_NODISCARD Reference<Entity> getEntity() const;

	protected:
		FRO_API Component();
		Component(Component const&) = default;
		Component(Component&&) noexcept = default;

		Component& operator=(Component const&) = default;
		Component& operator=(Component&&) noexcept = default;

	private:
		// these need to be initialized in the source file, since Entity.hpp cannot be
		// included here (Entity.hpp includes Component.hpp)
		EventListener<Entity, Component, std::type_index const> mOnComponentAttachEvent;
		EventListener<Entity, Component, std::type_index const> mOnComponentDetachEvent;

		Reference<Entity> mEntity{};
	};
}

#endif