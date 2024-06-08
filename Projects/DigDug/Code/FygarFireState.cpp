#include "FygarFireState.h"

#include "GameObject.h"
#include "InputManager.h"
#include "FygarFireState.h"

#pragma region Constructors/Destructor
fro::FygarFireState::FygarFireState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethod
fro::Reference<fro::State> fro::FygarFireState::update(float const deltaSeconds)
{
	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds >= m_ChargeTimeSeconds)
		return {};

	return {};
}
#pragma endregion PublicMethod