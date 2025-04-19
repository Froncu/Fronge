#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

#include "core.hpp"
#include "froch.hpp"
#include "identifier/id.hpp"
#include "utility/unique_pointer.hpp"
#include "user_input.hpp"

struct SDL_Gamepad;

namespace fro
{
   class Gamepad final : public Referenceable
   {
      friend class InputManager;

      public:
         Gamepad(Gamepad const&) = delete;
         Gamepad(Gamepad&&) = default;

         virtual ~Gamepad() override = default;

         Gamepad& operator=(Gamepad const&) = delete;
         Gamepad& operator=(Gamepad&&) = default;

      private:
         FRO_API explicit Gamepad(ID::InternalValue id);

         UniquePointer<SDL_Gamepad> native_gamepad_;
   };
}

#endif