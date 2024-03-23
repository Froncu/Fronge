#include "KeyInput.h"

#pragma region Constructors/Destructor
fro::KeyInput::KeyInput(SDL_Scancode key, State state)
	: key{ key }
	, state{ state }
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
bool fro::KeyInput::operator<(const KeyInput& otherKeyInput) const
{
	const size_t thisKeyInputStateMask{ static_cast<size_t>(this->state) << 32 };
	const size_t otherKeyInputStateMask{ static_cast<size_t>(otherKeyInput.state) << 32 };

	return
		(thisKeyInputStateMask | this->key) <
		(otherKeyInputStateMask | otherKeyInput.key);
};
#pragma endregion Operators