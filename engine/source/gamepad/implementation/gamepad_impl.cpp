#include "froch.hpp"

#include "GamepadImpl.hpp"

#include <SDL_gamecontroller.h>

namespace fro
{
   Gamepad::Implementation::Implementation(std::int32_t const deviceID)
      : mSDLGameController{ SDL_GameControllerOpen(deviceID), SDL_GameControllerClose }
   {
      if (mSDLGameController.get() == nullptr)
         exception("failed to open SDL_GameControllerOpen with device ID {} ({})",
            deviceID, SDL_GetError());
   }

   SDL_GameController* Gamepad::Implementation::getSDLGameController() const
   {
      return mSDLGameController.get();
   }

   std::int32_t Gamepad::Implementation::getID() const
   {
      return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(mSDLGameController.get()));
   }
}