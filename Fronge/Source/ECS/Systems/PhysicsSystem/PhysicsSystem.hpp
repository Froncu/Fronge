#if not defined PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Core.hpp"
#include "ECS/Components/Transform/Transform.hpp"
#include "ECS/Components/Rigidbody/Rigidbody.hpp"
#include "ECS/Group.hpp"

namespace fro
{
	class PhysicsSystem final
	{
	public:
		class Implementation;

		PhysicsSystem() = default;
		PhysicsSystem(PhysicsSystem const&) = default;
		PhysicsSystem(PhysicsSystem&&) noexcept = default;

		~PhysicsSystem() = default;

		PhysicsSystem& operator=(PhysicsSystem const&) = default;
		PhysicsSystem& operator=(PhysicsSystem&&) noexcept = default;

		FRO_API void onFixedUpdate(double const fixedDeltaSeconds);

	private:
		Group<Transform, Rigidbody> const mGroup{};
		std::vector<Vector2<double>> mPreviousTranslations{};
	};
}

#endif