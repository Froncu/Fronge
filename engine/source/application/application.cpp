#include <SDL3/SDL.h>

#include "application.hpp"

namespace fro
{
   SDL_InitFlags constexpr INITIALIZATION_FLAGS{
      SDL_INIT_EVENTS |
      SDL_INIT_VIDEO |
      SDL_INIT_GAMEPAD
   };

   Application::Application()
   {
      SDL_InitSubSystem(INITIALIZATION_FLAGS);
   }

   Application::Application(Application const&)
      : Application()
   {
   }

   Application::Application(Application&&) noexcept
      : Application()
   {
   }

   Application::~Application()
   {
      SDL_QuitSubSystem(INITIALIZATION_FLAGS);
   }
}