#include "FPSCounter.h"

#include "Timer.h"
#include "GameObject.h"
#include "Text.h"

#include <format>

#pragma region Constructors/Destructor
fro::FPSCounter::FPSCounter(GameObject const& parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::FPSCounter::update()
{
	float constexpr maxElapsedSeconds{ 0.2f };

	++m_Ticks;
	m_ElapsedSeconds += Timer::getInstance().getDeltaSeconds();
	if (m_ElapsedSeconds >= maxElapsedSeconds)
	{
		getParentingGameObject().getComponent<Text>()->setText(std::format("{:.1f}", m_Ticks / m_ElapsedSeconds));

		m_Ticks = 0;
		m_ElapsedSeconds -= maxElapsedSeconds;
	};
}
#pragma endregion PublicMethods