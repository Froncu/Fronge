#if not defined fro_RIGID_BODY_H
#define fro_RIGID_BODY_H

#include "Component.h"
#include "Defines.hpp"
#include "Event.hpp"

#include <vec2.hpp>

#include <set>
#include <vector>
#include <xstring>

namespace fro
{
	class RigidBody final : public Component
	{
		friend class PhysicsManager;

	public:
		RigidBody(Reference<GameObject> const parentingGameObject);

		virtual ~RigidBody() override;

		void setColliderSize(glm::vec2 const size);

		fro_NODISCARD glm::vec2 getColliderSize() const;
		fro_NODISCARD bool isOverlapping(Reference<RigidBody> const other) const;
		fro_NODISCARD bool isOverlapping(std::string_view const tag) const;
		fro_NODISCARD Reference<RigidBody> getOverlapping(std::string_view const tag) const;

		Event<Reference<RigidBody>> beginOverlap;
		Event<Reference<RigidBody>> endOverlap;

		glm::vec2 velocity{};

	private:
		struct RigidBodyComparer final
		{
			using is_transparent = std::true_type;

			fro_NODISCARD bool operator()(Reference<RigidBody> const body1, Reference<RigidBody> const body2) const;
			fro_NODISCARD bool operator()(Reference<RigidBody> const body, std::string_view const tag) const;
			fro_NODISCARD bool operator()(std::string_view const tag, Reference<RigidBody> const body) const;
		};

		RigidBody(RigidBody const&) = delete;
		RigidBody(RigidBody&&) noexcept = delete;

		RigidBody& operator=(RigidBody const&) = delete;
		RigidBody& operator=(RigidBody&&) noexcept = delete;

		void onActiveStateChanged(bool const isActive);

		std::set<Reference<RigidBody>, RigidBodyComparer> m_sOverlappingBodies{};

		glm::vec2 m_ColliderSize{};
	};
}

#endif