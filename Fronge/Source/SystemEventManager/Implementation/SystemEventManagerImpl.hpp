#ifndef SYSTEM_EVENT_MANAGER_IMPL_HPP
#define SYSTEM_EVENT_MANAGER_IMPL_HPP

#include <SDL_stdinc.h>

#include "SystemEventManager/SystemEventManager.hpp"

struct SDL_ControllerAxisEvent;
struct SDL_ControllerButtonEvent;
struct SDL_ControllerDeviceEvent;
struct SDL_KeyboardEvent;
struct SDL_WindowEvent;

namespace fro
{
   class SystemEventManager::Implementation final
   {
      public:
         Implementation() = default;
         Implementation(Implementation const&) = delete;
         Implementation(Implementation&&) noexcept = delete;

         ~Implementation() = default;

         Implementation& operator=(Implementation const&) = delete;
         Implementation& operator=(Implementation&&) noexcept = delete;

         void dispatchSDLWindowEvent(SystemEventManager& systemEventManager, SDL_WindowEvent const& SDLEvent);
         void dispatchSDLKeyboardEvent(SystemEventManager& systemEventManager, SDL_KeyboardEvent const& SDLEvent);
         void dispatchSDLControllerDeviceEvent(SystemEventManager& systemEventManager,
            SDL_ControllerDeviceEvent const& SDLEvent);
         void dispatchSDLControllerButtonEvent(SystemEventManager& systemEventManager,
            SDL_ControllerButtonEvent const& SDLEvent);
         void dispatchSDLControllerAxisEvent(SystemEventManager& systemEventManager,
            SDL_ControllerAxisEvent const& SDLEvent);
         void resetOppositeAxis(SystemEventManager& systemEventManager, SDL_ControllerAxisEvent const& SDLEvent);

      private:
         Sint16 mPreviousRightStickX{};
         Sint16 mPreviousRightStickY{};
         Sint16 mPreviousLeftStickX{};
         Sint16 mPreviousLeftStickY{};
   };
}

#endif