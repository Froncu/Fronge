#include "StateMachine.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::StateMachine::StateMachine(Reference<GameObject> const parentingGameObject)
	: Behaviour(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::StateMachine::fixedUpdate(float const fixedDeltaSeconds)
{
	if (not m_CurrentState.valid())
		return;

	updateState(m_CurrentState.get().fixedUpdate(fixedDeltaSeconds));
}

void fro::StateMachine::update(float const deltaSeconds)
{
	if (not m_CurrentState.valid())
		return;

	updateState(m_CurrentState.get().update(deltaSeconds));
}

void fro::StateMachine::lateUpdate(float const deltaSeconds)
{
	if (not m_CurrentState.valid())
		return;

	updateState(m_CurrentState.get().lateUpdate(deltaSeconds));
}

void fro::StateMachine::setCurrentState(Reference<State> const newState)
{
	if (m_CurrentState.valid())
		m_CurrentState.get().exit(newState);

	if (newState.valid())
		newState.get().enter(m_CurrentState);

	m_CurrentState = std::move(newState);

}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::StateMachine::updateState(Reference<State>&& newState)
{
	if (not newState.valid())
		return;

	setCurrentState(std::move(newState));
}
#pragma endregion PrivateMethods