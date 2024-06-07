#if not defined fro_COMPONENT_H
#define fro_COMPONENT_H

#include "Defines.hpp"
#include "Reference.hpp"

namespace fro
{
	class GameObject;

	class Component : public BaseReferencable
	{
	public:
		virtual ~Component() = default;

		Reference<GameObject> const parentingGameObject;

	protected:
		Component(Reference<GameObject> const parentingGameObject);

	private:
		Component(Component const&) = delete;
		Component(Component&&) noexcept = delete;

		Component& operator=(Component const&) = delete;
		Component& operator=(Component&&) noexcept = delete;
	};
}

#endif