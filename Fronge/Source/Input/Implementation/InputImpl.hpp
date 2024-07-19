#if not defined INPUT_IMPL_HPP
#define INPUT_IMPL_HPP

#include "Core.hpp"
#include "Input/Input.hpp"

#include <SDL_keycode.h>
#include <SDL_gamecontroller.h>

namespace fro
{
	FRO_API FRO_NODISCARD constexpr Key convertSDLKeyCode(SDL_Keycode const keyCode);
	FRO_API FRO_NODISCARD constexpr GamepadButton convertSDLControllerButton(Uint8 const button);
	FRO_API FRO_NODISCARD constexpr std::pair<GamepadAxis, double> convertSDLControllerAxis(Uint8 const axis, Sint16 const value);
}

#endif