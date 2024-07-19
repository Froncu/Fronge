#if not defined INPUT_IMPL_HPP
#define INPUT_IMPL_HPP

#include "Core.hpp"
#include "InputManager/Input/Input.hpp"

#include <SDL_keycode.h>

namespace fro
{
	FRO_API FRO_NODISCARD constexpr Key convertSDLKeyCode(SDL_Keycode const keyCode);
}

#endif