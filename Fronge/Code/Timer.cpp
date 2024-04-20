#include "Timer.h"

#include <chrono>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(Timer)
{
}

fro_GENERATED_SINGLETON_DESTRUCTOR(Timer)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Timer::update()
{
	static auto oldTimePoint{ std::chrono::steady_clock::now() };

	const auto currentTimePoint{ std::chrono::steady_clock::now() };
	m_DeltaSeconds = std::chrono::duration<float>(currentTimePoint - oldTimePoint).count();
	oldTimePoint = currentTimePoint;
}

fro_NODISCARD_GETTER float fro::Timer::getDeltaSeconds() const
{
	return m_DeltaSeconds;
}
#pragma endregion PublicMethods