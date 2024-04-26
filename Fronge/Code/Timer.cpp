#include "Timer.h"

#include <chrono>

#pragma region PublicMethods
void fro::Timer::update()
{
	static auto oldTimePoint{ std::chrono::steady_clock::now() };

	const auto currentTimePoint{ std::chrono::steady_clock::now() };
	m_DeltaSeconds = std::chrono::duration<float>(currentTimePoint - oldTimePoint).count();
	oldTimePoint = currentTimePoint;
}

fro_NODISCARD float fro::Timer::getDeltaSeconds() const
{
	return m_DeltaSeconds;
}
#pragma endregion PublicMethods