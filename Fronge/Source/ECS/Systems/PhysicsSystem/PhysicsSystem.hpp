#if not defined PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Core.hpp"
#include "ECS/Components/Transform/Transform.hpp"
#include "ECS/Components/Rigidbody/Rigidbody.hpp"
#include "ECS/Group.hpp"
#include "Renderer/Renderer.hpp"

namespace fro
{
	class PhysicsSystem final
	{
	public:
		class Implementation;

		FRO_API static void onFixedUpdate(double const fixedDeltaSeconds);
		FRO_API static void onRender(Renderer const& renderer);

		FRO_API static void setGravity(Vector2<double> const gravity);

		FRO_API static std::int32_t sVelocityIterations;
		FRO_API static std::int32_t sPositionIterations;

	private:
		static Group<Transform, Rigidbody> sGroup;

		PhysicsSystem() = delete;
		PhysicsSystem(PhysicsSystem const&) = delete;
		PhysicsSystem(PhysicsSystem&&) noexcept = delete;

		~PhysicsSystem() = delete;

		PhysicsSystem& operator=(PhysicsSystem const&) = delete;
		PhysicsSystem& operator=(PhysicsSystem&&) noexcept = delete;
	};
}

#endif