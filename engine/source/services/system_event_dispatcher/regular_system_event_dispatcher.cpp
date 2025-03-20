#include <SDL3/SDL.h>

#include "regular_system_event_dispatcher.hpp"

namespace fro
{
   RegularSystemEventManager::RegularSystemEventManager()
   {
      SDL_InitSubSystem(SDL_INIT_EVENTS);
   }

   RegularSystemEventManager::~RegularSystemEventManager()
   {
      SDL_QuitSubSystem(SDL_INIT_EVENTS);
   }

   void RegularSystemEventManager::poll_events()
   {
      SDL_Event native_event;
      while (SDL_PollEvent(&native_event))
         switch (native_event.type)
         {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
               RenderContextCloseEvent const event{ .id{ native_event.window.windowID } };
               render_context_event.notify(event);
               break;
            }

            default:
               break;
         }
   }
}