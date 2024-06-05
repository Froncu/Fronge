#if not defined fro_PHYSICS_MANAGER_H
#define fro_PHYSICS_MANAGER_H

#include "Singleton.hpp"

#include <b2_world.h>

#include <memory>

namespace fro
{
	class PhysicsManager final : public Singleton<PhysicsManager>, public b2ContactListener
	{
		friend class Singleton<PhysicsManager>;

		friend class RigidBody;

	public:
		~PhysicsManager() = default;

		void update(float const fixedDeltaSeconds);

	private:
		PhysicsManager();
		PhysicsManager(PhysicsManager const&) = delete;
		PhysicsManager(PhysicsManager&&) noexcept = delete;

		PhysicsManager& operator=(PhysicsManager const&) = delete;
		PhysicsManager& operator=(PhysicsManager&&) noexcept = delete;

		virtual void BeginContact(b2Contact* const pContact) override;
		virtual void EndContact(b2Contact* const pContact) override;

		b2World m_World{ { 0.0f, 9.81f } };
	};
}

#endif