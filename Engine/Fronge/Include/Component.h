#if not defined fro_COMPONENT_H
#define fro_COMPONENT_H

#include "Defines.hpp"

namespace fro
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		fro_NODISCARD GameObject& getParentingGameObject() const;

	protected:
		Component(GameObject& parentingGameObject);

	private:
		Component(Component const&) = delete;
		Component(Component&&) noexcept = delete;

		Component& operator=(Component const&) = delete;
		Component& operator=(Component&&) noexcept = delete;

		GameObject* m_pParentingGameObject;
	};
}

#endif