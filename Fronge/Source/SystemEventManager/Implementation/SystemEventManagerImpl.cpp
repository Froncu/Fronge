#include <SDL.h>

#include "froch.hpp"
#include "Input/Implementation/InputImpl.hpp"
#include "Logger/Logger.hpp"
#include "SystemEventManagerImpl.hpp"

namespace fro
{
   void SystemEventManager::Implementation::dispatchSDLWindowEvent(SystemEventManager& systemEventManager,
      SDL_WindowEvent const& SDLEvent)
   {
      // SDL's ID's for windows start from 1, Fronge's start from 0, so we subtract 1
      switch (SDLEvent.event)
      {
         case SDL_WINDOWEVENT_CLOSE:
         {
            WindowCloseEvent event{ SDLEvent.windowID - 1 };
            Logger::info("dispatched {}!", event.getLogString());

            systemEventManager.mWindowEvent.notify(std::move(event));
            break;
         }
         case SDL_WINDOWEVENT_SIZE_CHANGED:
         {
            WindowResizeEvent event{ .ID{ SDLEvent.windowID - 1 }, .size{ SDLEvent.data1, SDLEvent.data2 } };

            Logger::info("dispatched {}!", event.getLogString());

            systemEventManager.mWindowEvent.notify(std::move(event));
            break;
         }
      }
   }

   void SystemEventManager::Implementation::dispatchSDLKeyboardEvent(SystemEventManager& systemEventManager,
      SDL_KeyboardEvent const& SDLEvent)
   {
      switch (SDLEvent.type)
      {
         case SDL_KEYDOWN:
         {
            KeyDownEvent event{ convertSDLKeyCode(SDLEvent.keysym.sym) };

            systemEventManager.mInputEvent.notify(std::move(event));
            break;
         }
         case SDL_KEYUP:
         {
            KeyUpEvent event{ convertSDLKeyCode(SDLEvent.keysym.sym) };

            systemEventManager.mInputEvent.notify(std::move(event));
            break;
         }
      }
   }

   void SystemEventManager::Implementation::dispatchSDLControllerDeviceEvent(SystemEventManager& systemEventManager,
      SDL_ControllerDeviceEvent const& SDLEvent)
   {
      switch (SDLEvent.type)
      {
         case SDL_CONTROLLERDEVICEADDED:
         {
            GamepadConnectedEvent event{ SDLEvent.which };
            Logger::info("dispatched {}!", event.getLogString());

            systemEventManager.mInputEvent.notify(std::move(event));
            break;
         }
         case SDL_CONTROLLERDEVICEREMOVED:
         {
            GamepadDisconnectedEvent event{ SDLEvent.which };
            Logger::warn("dispatched {}!", event.getLogString());

            systemEventManager.mInputEvent.notify(std::move(event));
            break;
         }
      }
   }

   void SystemEventManager::Implementation::dispatchSDLControllerButtonEvent(SystemEventManager& systemEventManager,
      SDL_ControllerButtonEvent const& SDLEvent)
   {
      switch (SDLEvent.type)
      {
         case SDL_CONTROLLERBUTTONDOWN:
         {
            GamepadButtonDownEvent event{ { SDLEvent.which, convertSDLControllerButton(SDLEvent.button) } };

            systemEventManager.mInputEvent.notify(std::move(event));
            break;
         }
         case SDL_CONTROLLERBUTTONUP:
         {
            GamepadButtonUpEvent event{ { SDLEvent.which, convertSDLControllerButton(SDLEvent.button) } };

            systemEventManager.mInputEvent.notify(std::move(event));
            break;
         }
      }
   }

   void SystemEventManager::Implementation::dispatchSDLControllerAxisEvent(SystemEventManager& systemEventManager,
      SDL_ControllerAxisEvent const& SDLEvent)
   {
      resetOppositeAxis(systemEventManager, SDLEvent);

      auto&& [input, value]{ convertSDLControllerAxis(SDLEvent.axis, SDLEvent.value) };

      GamepadAxisEvent event{ .input{ SDLEvent.which, input }, .value{ value } };

      systemEventManager.mInputEvent.notify(std::move(event));
   }

   // SDL treats horizontal and veritcal gamepad sticks as one value,
   // Fronge does not. This means that if a stick goes from e.g. 128 to -128,
   // Fronge is not notified that the opposite axis now should be 0. This fixes that.
   void SystemEventManager::Implementation::resetOppositeAxis(SystemEventManager& systemEventManager,
      SDL_ControllerAxisEvent const& SDLEvent)
   {
      Sint16* previousValue;
      switch (SDLEvent.axis)
      {
         case SDL_CONTROLLER_AXIS_RIGHTX:
            previousValue = &mPreviousRightStickX;
            break;

         case SDL_CONTROLLER_AXIS_RIGHTY:
            previousValue = &mPreviousRightStickY;
            break;

         case SDL_CONTROLLER_AXIS_LEFTX:
            previousValue = &mPreviousLeftStickX;
            break;

         case SDL_CONTROLLER_AXIS_LEFTY:
            previousValue = &mPreviousLeftStickY;
            break;

         default:
            return;
      }

      if (*previousValue < 0 and SDLEvent.value > 0 or *previousValue > 0 and SDLEvent.value < 0)
      {
         auto&& [oppositeInput, _]{ convertSDLControllerAxis(SDLEvent.axis, SDLEvent.value < 0 ? 1 : -1) };

         GamepadAxisEvent event{ .input{ SDLEvent.which, oppositeInput }, .value{} };

         systemEventManager.mInputEvent.notify(std::move(event));
      };

      *previousValue = SDLEvent.value;
   }
}