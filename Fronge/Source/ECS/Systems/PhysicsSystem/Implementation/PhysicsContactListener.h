#ifndef PHYSICS_CONTACT_LISTENER_HPP
#define PHYSICS_CONTACT_LISTENER_HPP

#include <box2d/b2_world_callbacks.h>

namespace fro
{
	class PhysicsContactListener : public b2ContactListener
	{
	public:
		PhysicsContactListener() = default;
		PhysicsContactListener(PhysicsContactListener const&) = default;
		PhysicsContactListener(PhysicsContactListener&&) noexcept = default;

		virtual ~PhysicsContactListener() override = default;

		PhysicsContactListener& operator=(PhysicsContactListener const&) = default;
		PhysicsContactListener& operator=(PhysicsContactListener&&) noexcept = default;

	private:
		virtual void BeginContact(b2Contact* const contact) override;
		virtual void EndContact(b2Contact* const contact) override;
	};
}

#endif