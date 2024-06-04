#include "FPSCounter.h"

#include <format>

#pragma region Constructors/Destructor
fro::FPSCounter::FPSCounter(Reference<GameObject> const parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::FPSCounter::update(float const deltaSeconds)
{
	float constexpr maxElapsedSeconds{ 0.2f };

	++m_Ticks;
	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds >= maxElapsedSeconds)
	{
		m_Text.get().setText(std::format("{:.1f}", m_Ticks / m_ElapsedSeconds));

		m_Ticks = 0;
		m_ElapsedSeconds -= maxElapsedSeconds;
	};
}
#pragma endregion PublicMethods