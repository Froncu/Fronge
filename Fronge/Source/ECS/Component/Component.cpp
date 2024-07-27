#include "froch.hpp"

#include "Component.hpp"

namespace fro
{
	Component::Component(Component&& other) noexcept
		: Referencable(std::move(other))
	{
	}

	Component& Component::operator=(Component&& other) noexcept
	{
		Referencable::operator=(std::move(other));

		return *this;
	}
}