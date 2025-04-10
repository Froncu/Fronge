#ifndef INPUT_CONVERSIONS_HPP
#define INPUT_CONVERSIONS_HPP

#include <SDL3/SDL.h>

#include "input.hpp"

namespace fro
{
   [[nodiscard]] Key convert_sdl_key_code(SDL_Keycode key_code);
   [[nodiscard]] GamepadButton convert_sdl_controller_button(Uint8 button);
   [[nodiscard]] GamepadAxis convert_sdl_controller_axis(Uint8 axis, Sint16 value);
   [[nodiscard]] std::pair<GamepadAxis, double> convert_sdl_controller_axis_value(Uint8 axis, Sint16 value);
}

#endif