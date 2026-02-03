#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "fronge/fronge.hpp"

namespace fro
{
   [[nodiscard]] Application* create_application();
}

SDL_AppResult SDL_AppInit(void** const appstate, int, char** const)
{
   *appstate = fro::create_application();
   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* const appstate)
{
   return
      static_cast<fro::Application*>(appstate)->tick()
         ? SDL_APP_CONTINUE
         : SDL_APP_SUCCESS;
}

SDL_AppResult SDL_AppEvent(void* const appstate, SDL_Event* const event)
{
   static_cast<fro::Application*>(appstate)->process_event(*event);
   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* const appstate, SDL_AppResult const)
{
   delete static_cast<fro::Application const*>(appstate);
   fro::Locator::remove_providers();
}