#include "PhysicsManager.h"

#pragma region PublicMethods
void fro::PhysicsManager::update(float const fixedDeltaSeconds)
{
	m_World.Step(fixedDeltaSeconds, 6, 2);
}
#pragma endregion PublicMethods