#if not defined PHYSICS_SYSTEM_IMPL_HPP
#define PHYSICS_SYSTEM_IMPL_HPP

#include "froch.hpp"

#include "ECS/Systems/PhysicsSystem/PhysicsSystem.hpp"

#include <box2d/b2_world.h>

namespace fro
{
	class PhysicsSystem::Implementation final
	{
	public:
		static b2World sWorld;

	private:
		Implementation() = delete;
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		~Implementation() = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;
	};
}

#endif