#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

#include "fronge/api.hpp"
#include "fronge/id.hpp"
#include "fronge/pch.hpp"
#include "fronge/unique_pointer.hpp"
#include "fronge/user_input.hpp"

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

         FRO_API [[nodiscard]] ID::InternalValue id() const;
         FRO_API [[nodiscard]] int user_input_id() const;

      private:
         FRO_API [[nodiscard]] static int user_input_id(ID::InternalValue id);

         FRO_API explicit Gamepad(ID::InternalValue id);

         void assign_user_input_id(int user_input_id) const;

         UniquePointer<SDL_Gamepad> native_gamepad_;
   };
}

#endif