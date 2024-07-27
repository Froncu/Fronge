#if not defined COMPONENT_HPP
#define COMPONENT_HPP

#include "Core.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	class Component : public Referencable
	{
	public:
		virtual ~Component() override = default;

	protected:
		Component() = default;
		Component(Component const&) = default;
		FRO_API Component(Component&& other) noexcept;

		Component& operator=(Component const&) = default;
		FRO_API Component& operator=(Component&& other) noexcept;
	};
}

#endif