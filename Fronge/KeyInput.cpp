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
	if (this->key != otherKeyInput.key)
		return this->key < otherKeyInput.key;

	return this->state < otherKeyInput.state;
};
#pragma endregion Operators