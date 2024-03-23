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
	const auto thisKeyInputStateMask{ static_cast<std::uint64_t>(this->state) << 32 };
	const auto otherKeyInputStateMask{ static_cast<std::uint64_t>(otherKeyInput.state) << 32 };

	return
		(thisKeyInputStateMask | this->key) <
		(otherKeyInputStateMask | otherKeyInput.key);
};
#pragma endregion Operators