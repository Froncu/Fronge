#include <SDL.h>

#include "froch.hpp"
#include "Implementation/SystemEventManagerImpl.hpp"
#include "SystemEventManager.hpp"

namespace fro
{
   SystemEventManager::SystemEventManager()
      : mImplementation{ std::make_unique<Implementation>() }
   {
      Logger::info("initialized SystemEventManager!");
   }

   SystemEventManager::~SystemEventManager()
   {
      Logger::info("shut down SystemEventManager!");
   }

   void SystemEventManager::pollEvents()
   {
      SDL_Event event;
      while (SDL_PollEvent(&event))
         switch (event.type)
         {
            case SDL_WINDOWEVENT:
               mImplementation->dispatchSDLWindowEvent(*this, event.window);
               break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
               // TODO: no support for repeated key down events
               if (event.key.repeat == 0)
                  mImplementation->dispatchSDLKeyboardEvent(*this, event.key);
               break;

            case SDL_CONTROLLERDEVICEADDED:
            case SDL_CONTROLLERDEVICEREMOVED:
               mImplementation->dispatchSDLControllerDeviceEvent(*this, event.cdevice);
               break;

            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
               mImplementation->dispatchSDLControllerButtonEvent(*this, event.cbutton);
               break;

            case SDL_CONTROLLERAXISMOTION:
               mImplementation->dispatchSDLControllerAxisEvent(*this, event.caxis);
               break;
         }
   }
}