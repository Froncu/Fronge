#if not defined fro_RIGID_BODY_H
#define fro_RIGID_BODY_H

#include "Component.h"
#include "Defines.hpp"

#include <vec2.hpp>

namespace fro
{
	class RigidBody final : public Component
	{		
	public:
		RigidBody(Reference<GameObject> const parentingGameObject);

		virtual ~RigidBody() override;

		void setColliderSize(glm::vec2 const size);
		fro_NODISCARD glm::vec2 getColliderSize() const;

		glm::vec2 velocity{};

	private:
		RigidBody(RigidBody const&) = delete;
		RigidBody(RigidBody&&) noexcept = delete;

		RigidBody& operator=(RigidBody const&) = delete;
		RigidBody& operator=(RigidBody&&) noexcept = delete;

		glm::vec2 m_ColliderSize{};
	};
}

#endif