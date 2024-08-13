#if not defined PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Core.hpp"
#include "ECS/Components/Transform/Transform.hpp"
#include "ECS/Components/Rigidbody/Rigidbody.hpp"
#include "ECS/Group.hpp"
#include "Events/Systems/EventDispatcher.hpp"
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
		FRO_API FRO_NODISCARD static std::pair<Reference<Rigidbody>, Reference<Collider>> raycast(Vector2<double> const from, Vector2<double> const to);

		FRO_API static EventDispatcher<Rigidbody, Rigidbody> sBeginContactEvent;
		FRO_API static EventDispatcher<Rigidbody, Rigidbody> sEndContactEvent;
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