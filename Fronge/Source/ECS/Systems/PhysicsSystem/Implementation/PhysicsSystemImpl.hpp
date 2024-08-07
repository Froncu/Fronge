#if not defined PHYSICS_SYSTEM_IMPL_HPP
#define PHYSICS_SYSTEM_IMPL_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "ECS/Systems/PhysicsSystem/PhysicsSystem.hpp"

#include <box2d/b2_world.h>

namespace fro
{
	class PhysicsDebugRenderer;

	class PhysicsSystem::Implementation final
	{
	public:
		FRO_API static b2World sWorld;
		FRO_API static std::unique_ptr<PhysicsDebugRenderer> const sDebugRenderer;

	private:
		static std::unique_ptr<PhysicsDebugRenderer> initializeDebugRenderer();

		Implementation() = delete;
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		~Implementation() = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;
	};
}

#endif