#if not defined fro_RIGD_BODY_H
#define fro_RIGD_BODY_H

#include "FixedBehaviour.h"
#include "Typenames.hpp"

#include <b2_body.h>
#include <vec2.hpp>

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <xstring>

namespace fro
{
	class RigidBody final : public FixedBehaviour
	{
		// TODO: not sure about this
		friend class PhysicsManager;
		// END TODO
		
	public:
		RigidBody(Reference<GameObject> const parentingGameObject);

		virtual ~RigidBody() override = default;

		virtual void fixedUpdate(float const fixedDeltaTime) override;

		void setType(b2BodyType const type);
		void setVelocity(glm::vec2 const& velocity);

		void addBoxCollider(std::string_view const name, glm::vec2 const& size);
		void removeBoxCollider(std::string_view const name);

	private:
		RigidBody(RigidBody const&) = delete;
		RigidBody(RigidBody&&) noexcept = delete;

		RigidBody& operator=(RigidBody const&) = delete;
		RigidBody& operator=(RigidBody&&) noexcept = delete;

		b2Body& createBody() const;

		b2Body& m_Body{ createBody() };
		std::map<std::string_view, b2Fixture*> m_spColliders{};

		std::set<RigidBody const*> m_spOverlappingRigidBodies{};
	};
}

#endif