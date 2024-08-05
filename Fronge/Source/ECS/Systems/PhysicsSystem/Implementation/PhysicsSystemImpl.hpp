#if not defined PHYSICS_SYSTEM_IMPL_HPP
#define PHYSICS_SYSTEM_IMPL_HPP

#include "ECS/Systems/PhysicsSystem/PhysicsSystem.hpp"

namespace fro
{
	class PhysicsSystem::Implementation final
	{
	public:
		Implementation() = default;

		~Implementation() = default;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;
	};
}

#endif