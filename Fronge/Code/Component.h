#pragma once

#include "Defines.hpp"

namespace fro
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		fro_NODISCARD const GameObject& getParentingGameObject() const;

	protected:
		Component(const GameObject& parentingGameObject);

	private:
		Component(const Component&) = delete;
		Component(Component&&) noexcept = delete;

		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) noexcept = delete;

		const GameObject& m_ParentingGameObject;
	};
}