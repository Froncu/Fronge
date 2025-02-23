#ifndef PHYSICS_SYSTEM_IMPL_HPP
#define PHYSICS_SYSTEM_IMPL_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "ECS/Systems/PhysicsSystem/PhysicsSystem.hpp"
#include "PhysicsContactListener.h"
#include "PhysicsDebugRenderer.hpp"

#include <box2d/b2_world.h>

namespace fro
{
	class PhysicsSystem::Implementation final
	{
	public:
		FRO_API static b2World sWorld;
		FRO_API static PhysicsDebugRenderer sDebugRenderer;
		FRO_API static PhysicsContactListener sContactListener;

	private:
		static PhysicsDebugRenderer createDebugRenderer();
		static PhysicsContactListener createContactListener();

		Implementation() = delete;
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		~Implementation() = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;
	};
}

#endif