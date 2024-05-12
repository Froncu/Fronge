#include "StateMachine.h"

#pragma region Constructors/Destructor
fro::StateMachine::StateMachine(GameObject const& parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::StateMachine::update()
{
	if (not m_pCurrentState.get())
		return;

	std::unique_ptr pNewState{ m_pCurrentState->update() };
	if (not pNewState.get())
		return;

	m_pCurrentState->exit(pNewState);
	pNewState->enter(m_pCurrentState);

	m_pCurrentState = std::move(pNewState);
}
#pragma endregion PublicMethods