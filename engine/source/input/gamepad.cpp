#include <SDL3/SDL_gamepad.h>

#include "froch.hpp"
#include "gamepad.hpp"
#include "services/input_manager/input_manager.hpp"
#include "utility/assert.hpp"

namespace fro
{
   Gamepad::Gamepad(ID::InternalValue const id)
      : native_gamepad_{
         [](ID::InternalValue const id)
         {
            SDL_Gamepad* native_gamepad{ SDL_OpenGamepad(id) };
            assert(native_gamepad, "failed to open a Gamepad with ID {} ({})",
               id, SDL_GetError());

            return native_gamepad;
         }(id),
         SDL_CloseGamepad
      }
   {
   }

   // bool Gamepad::rumble(std::uint16_t const low_frequency, std::uint16_t const high_frequency,
   //    std::uint32_t const duration_milliseconds) const
   // {
   //    if (SDL_RumbleGamepad(native_gamepad_.get(), low_frequency, high_frequency, duration_milliseconds))
   //       return true;
   //
   //    Locator::get<Logger>().warning("failed to rumble Gamepad{} ({})",
   //       id(), SDL_GetError());
   //
   //    return false;
   // }
}