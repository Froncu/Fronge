#include "ButtonInput.h"

#pragma region Constructors/Destructor
fro::ButtonInput::ButtonInput(Button button, State state)
	: m_Button{ button }
	, m_State{ state }
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
bool fro::ButtonInput::operator<(const ButtonInput& otherButtonInput) const
{
	if (this->m_Button != otherButtonInput.m_Button)
		return this->m_Button < otherButtonInput.m_Button;

	return this->m_State < otherButtonInput.m_State;
};
#pragma endregion Operators