#include "PhysicsManager.h"

#pragma region PublicMethods
void fro::PhysicsManager::update(float const deltaSeconds)
{
	for (m_ElapsedSeconds += deltaSeconds;
		m_ElapsedSeconds >= m_FixedDeltaSeconds;
		m_ElapsedSeconds -= m_FixedDeltaSeconds)
		m_World.Step(m_FixedDeltaSeconds, 6, 2);
}

void fro::PhysicsManager::setFixedFrameRate(int const fixedFrameRate)
{
	m_FixedDeltaSeconds = 1.0f / fixedFrameRate;
}
#pragma endregion PublicMethods