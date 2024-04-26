#include "Timer.h"

#pragma region PublicMethods
void fro::Timer::update()
{
	auto const currentTimePoint{ std::chrono::steady_clock::now() };
	m_DeltaSeconds = std::chrono::duration<float>(currentTimePoint - m_OldTimePoint).count();
	m_OldTimePoint = currentTimePoint;
}

fro_NODISCARD float fro::Timer::getDeltaSeconds() const
{
	return m_DeltaSeconds;
}
#pragma endregion PublicMethods