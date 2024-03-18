#include "Timer.h"

#include <chrono>

#pragma region StaticDataMembers
float fro::Timer::m_DELTA_SECONDS{};
#pragma endregion StaticDataMembers



#pragma region PublicMethods
float fro::Timer::getDeltaSeconds()
{
	return m_DELTA_SECONDS;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Timer::update()
{
	static auto oldTimePoint{ std::chrono::steady_clock::now() };

	const auto currentTimePoint{ std::chrono::steady_clock::now() };
	m_DELTA_SECONDS = std::chrono::duration<float>(currentTimePoint - oldTimePoint).count();
	oldTimePoint = currentTimePoint;
}
#pragma endregion PrivateMethods