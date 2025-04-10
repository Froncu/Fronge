#include <SDL3/SDL_gamepad.h>

#include "froch.hpp"
#include "gamepad.hpp"
#include "utility/assert.hpp"

namespace fro
{
   Gamepad::Gamepad(std::uint32_t const id)
      : id_{ id }
      , native_gamepad_{
         [](std::uint32_t const id)
         {
            SDL_Gamepad* native_gamepad{ SDL_OpenGamepad(id) };
            assert(native_gamepad, "failed to open a Gamepad with ID {} ({})",
               id, SDL_GetError());

            return native_gamepad;
         }(id_),
         SDL_CloseGamepad
      }
   {
   }

   Gamepad::Gamepad(Gamepad const& other)
      : Gamepad(other.id_)
   {
   }

   Gamepad& Gamepad::operator=(Gamepad const& other)
   {
      if (this == &other)
         return *this;

      return *this = Gamepad{ other.id_ };
   }

   std::uint32_t Gamepad::id() const
   {
      return id_;
   }
}