#include "ButtonInput.h"

#pragma region Constructors/Destructor
fro::ButtonInput::ButtonInput(Button const button, State const state)
	: m_Button{ button }
	, m_State{ state }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::ButtonInput::State fro::ButtonInput::getState() const
{
	return m_State;
}
#pragma endregion PublicMethods



#pragma region Operators
bool fro::ButtonInput::operator<(ButtonInput const& otherButtonInput) const
{
	if (m_Button != otherButtonInput.m_Button)
		return m_Button < otherButtonInput.m_Button;

	return m_State < otherButtonInput.m_State;
}
#pragma endregion Operators