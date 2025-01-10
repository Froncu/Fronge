#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class Component : public Referencable
	{
	public:
		virtual ~Component() override = default;

	protected:
		Component() = default;
		Component(Component const&) = default;
		Component(Component&&) noexcept = default;

		Component& operator=(Component const&) = default;
		Component& operator=(Component&&) noexcept = default;
	};
}

#endif