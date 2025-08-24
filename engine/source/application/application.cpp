#include <SDL3/SDL.h>

#include "application.hpp"
#include "services/locator.hpp"
#include "utility/runtime_assert.hpp"

namespace fro
{
   SDL_InitFlags constexpr INITIALIZATION_FLAGS{
      SDL_INIT_EVENTS |
      SDL_INIT_VIDEO |
      SDL_INIT_GAMEPAD
   };

   Application::Application()
   {
      ImGui::StyleColorsDark();

      bool const succeeded{ SDL_InitSubSystem(INITIALIZATION_FLAGS) };
      runtime_assert(succeeded, "failed to initialize SDL subsystems ({})",
         SDL_GetError());
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
      Locator::remove_providers();

      SDL_QuitSubSystem(INITIALIZATION_FLAGS);
      SDL_Quit();
   }
}