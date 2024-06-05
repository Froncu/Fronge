#if not defined fro_RIGD_BODY_H
#define fro_RIGD_BODY_H

#include "Component.h"
#include "Event.hpp"
#include "Typenames.hpp"

#include <b2_body.h>
#include <vec2.hpp>

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace fro
{
	class RigidBody final : public Component
	{
		friend class PhysicsManager;
		
	public:
		enum class Type
		{
			dynamicBody,
			staticBody
		};

		RigidBody(Reference<GameObject> const parentingGameObject);

		virtual ~RigidBody() override = default;

		void setGravityScale(float const scale);
		void setType(Type const type);
		void setVelocity(glm::vec2 velocity);

		std::set<Reference<RigidBody>> const& getOverlappingRigidBodies() const;

		void addBoxShape(std::string name, glm::vec2 const& size, bool shouldCollide = false);
		void removeBoxShape(std::string const& name);

		Event<Reference<RigidBody>> beginOverlap{};
		Event<Reference<RigidBody>> endOverlap{};

	private:
		RigidBody(RigidBody const&) = delete;
		RigidBody(RigidBody&&) noexcept = delete;

		RigidBody& operator=(RigidBody const&) = delete;
		RigidBody& operator=(RigidBody&&) noexcept = delete;

		b2Body& createBody() const;

		b2Body& m_Body{ createBody() };
		std::map<std::string, b2Fixture*> m_mpColliders{};

		std::set<Reference<RigidBody>> m_sOverlappingRigidBodies{};
	};
}

#endif