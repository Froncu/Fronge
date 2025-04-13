#include <SDL3/SDL_gamepad.h>

#include "froch.hpp"
#include "gamepad.hpp"
#include "utility/assert.hpp"

namespace fro
{
   Gamepad::Gamepad(std::uint32_t const id)
      : native_gamepad_{
         [](std::uint32_t const id)
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

   Gamepad::Gamepad(Gamepad const& other)
      : Gamepad(other.id())
   {
   }

   Gamepad& Gamepad::operator=(Gamepad const& other)
   {
      if (this == &other)
         return *this;

      return *this = Gamepad{ other.id() };
   }

   bool Gamepad::rumble(std::uint16_t const low_frequency, std::uint16_t const high_frequency,
      std::uint32_t const duration_milliseconds)
   {
      if (SDL_RumbleGamepad(native_gamepad_.get(), low_frequency, high_frequency, duration_milliseconds))
         return true;

      Locator::get<Logger>().warning("failed to rumble Gamepad{} ({})", id(), SDL_GetError());
      return false;
   }

   std::uint32_t Gamepad::id() const
   {
      std::uint32_t const id{ SDL_GetGamepadID(native_gamepad_.get()) };
      assert(id, "failed to get the ID of a Gamepad ({})",
         SDL_GetError());

      return id;
   }
}