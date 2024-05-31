#include "StateMachine.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::StateMachine::StateMachine(Reference<GameObject> const parentingGameObject)
	: Behaviour(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::StateMachine::update(float const deltaSeconds)
{
	if (not m_pCurrentState.get())
		return;

	std::unique_ptr pNewState{ m_pCurrentState->update(deltaSeconds) };
	if (not pNewState.get())
		return;

	m_pCurrentState->exit(pNewState);
	setCurrentState(std::move(pNewState));
}

void fro::StateMachine::setCurrentState(std::unique_ptr<State>&& pNewState)
{
	pNewState->enter(m_pCurrentState);
	m_pCurrentState = std::move(pNewState);
}
#pragma endregion PublicMethods