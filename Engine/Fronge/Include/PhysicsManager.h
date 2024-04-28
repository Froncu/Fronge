#if not defined fro_PHYSICS_MANAGER_H
#define fro_PHYSICS_MANAGER_H

#include "Singleton.hpp"

#include <box2d.h>

#include <memory>

namespace fro
{
	class PhysicsManager final : public Singleton<PhysicsManager>
	{
	public:
		PhysicsManager() = default;

		~PhysicsManager() = default;

		void update(float const deltaSeconds);

		void setFixedFrameRate(int const fixedFrameRate);

	private:
		PhysicsManager(PhysicsManager const&) = delete;
		PhysicsManager(PhysicsManager&&) noexcept = delete;

		PhysicsManager& operator=(PhysicsManager const&) = delete;
		PhysicsManager& operator=(PhysicsManager&&) noexcept = delete;

		b2World m_World{ { 0.0f, 10.0f} };

		float m_ElapsedSeconds{};
		float m_FixedDeltaSeconds{ 1 / 30.0f };
	};
}

#endif