#pragma once

#include "Defines.hpp"

namespace fro
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		fro_NODISCARD GameObject const& getParentingGameObject() const;

	protected:
		Component(GameObject const& parentingGameObject);

	private:
		Component(Component const&) = delete;
		Component(Component&&) noexcept = delete;

		Component& operator=(Component const&) = delete;
		Component& operator=(Component&&) noexcept = delete;

		GameObject const& m_ParentingGameObject;
	};
}