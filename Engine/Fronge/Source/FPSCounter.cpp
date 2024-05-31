#include "FPSCounter.h"

#include "GameObject.h"
#include "Text.h"

#include <format>

#pragma region Constructors/Destructor
fro::FPSCounter::FPSCounter(Reference<GameObject> const parentingGameObject)
	: Behaviour(std::move(parentingGameObject))
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
		m_ParentingGameObject.get().getComponent<Text>()->setText(std::format("{:.1f}", m_Ticks / m_ElapsedSeconds));

		m_Ticks = 0;
		m_ElapsedSeconds -= maxElapsedSeconds;
	};
}
#pragma endregion PublicMethods