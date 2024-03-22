#include "FPSCounter.h"

#include "Timer.h"
#include "GameObject.h"
#include "Text.h"

#include <format>

#pragma region Constructors/Destructor
fro_GENERATED_BEHAVIOUR_CONSTRUCTOR(FPSCounter)
{
}

fro_GENERATED_BEHAVIOUR_DESTRUCTOR(FPSCounter)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods

#pragma endregion PublicMethods



#pragma region PrivateMethods
fro_GENERATED_BEHAVIOUR_UPDATE(FPSCounter)
{
	constexpr float maxElapsedSeconds{ 0.2f };

	++m_Ticks;
	m_ElapsedSeconds += Timer::getDeltaSeconds();
	if (m_ElapsedSeconds >= maxElapsedSeconds)
	{
		getParentingGameObject().getComponent<Text>()->setText(std::format("{:.1f}", m_Ticks / m_ElapsedSeconds));

		m_Ticks = 0;
		m_ElapsedSeconds -= maxElapsedSeconds;
	};
}
#pragma endregion PrivateMethods