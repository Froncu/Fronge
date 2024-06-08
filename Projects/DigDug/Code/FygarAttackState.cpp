#include "FygarAttackState.h"

#include "GameObject.h"
#include "InputManager.h"
#include "FygarAttackState.h"

#pragma region Constructors/Destructor
fro::FygarAttackState::FygarAttackState(Reference<GameObject> const parentingGameObject)
	: State(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethod
fro::Reference<fro::State> fro::FygarAttackState::update(float const deltaSeconds)
{
	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds >= m_ChargeTimeSeconds)
		return {};

	return {};
}
#pragma endregion PublicMethod