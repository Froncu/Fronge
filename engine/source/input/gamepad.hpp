#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

#include "core.hpp"
#include "froch.hpp"
#include "reference/referenceable.hpp"
#include "utility/unique_pointer.hpp"

struct SDL_Gamepad;

namespace fro
{
   class Gamepad final : public Referenceable
   {
      friend class InputManager;

      public:
         FRO_API explicit Gamepad(std::uint32_t id);
         FRO_API Gamepad(Gamepad const& other);
         FRO_API Gamepad(Gamepad&&) = default;

         FRO_API virtual ~Gamepad() override = default;

         FRO_API Gamepad& operator=(Gamepad const& other);
         FRO_API Gamepad& operator=(Gamepad&&) = default;

         FRO_API std::uint32_t id() const;

      private:

         std::uint32_t id_;
         UniquePointer<SDL_Gamepad> native_gamepad_;
   };
}

#endif