#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "fronge/fronge.hpp"

namespace fro
{
   [[nodiscard]] Application* create_application(std::vector<std::string_view> arguments);
}

SDL_AppResult SDL_AppInit(void** const appstate, int const argc, char** const argv) try
{
   *appstate = fro::create_application({ argv, argv + argc });
   return SDL_APP_CONTINUE;
}
catch (std::exception const& exception)
{
   fro::Locator::get<fro::Logger>().error(std::format("failed to initialize application ({})", exception.what()));
   return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppIterate(void* const appstate) try
{
   return
      static_cast<fro::Application*>(appstate)->tick()
         ? SDL_APP_CONTINUE
         : SDL_APP_SUCCESS;
}
catch (std::exception const& exception)
{
   fro::Locator::get<fro::Logger>().error(std::format("failed to iterate application ({})", exception.what()));
   return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppEvent(void* const appstate, SDL_Event* const event) try
{
   static_cast<fro::Application*>(appstate)->process_event(*event);
   return SDL_APP_CONTINUE;
}
catch (std::exception const& exception)
{
   fro::Locator::get<fro::Logger>().error(std::format("failed to process application event ({})", exception.what()));
   return SDL_APP_FAILURE;
}

void SDL_AppQuit(void* const appstate, SDL_AppResult const)
{
   delete static_cast<fro::Application*>(appstate);
}