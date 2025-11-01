#include <SDL3/SDL.h>

#include "fronge/application.hpp"
#include "fronge/ecs/entity.hpp"
#include "fronge/locator/locator.hpp"
#include "fronge/locator/services/editor_ui.hpp"
#include "fronge/locator/services/input_manager.hpp"
#include "fronge/locator/services/renderer.hpp"
#include "fronge/locator/services/scene_manager.hpp"
#include "fronge/locator/services/system_event_dispatcher.hpp"
#include "fronge/locator/services/window.hpp"
#include "fronge/utility/runtime_assert.hpp"

namespace fro
{
   SDL_InitFlags constexpr INITIALIZATION_FLAGS{
      SDL_INIT_EVENTS |
      SDL_INIT_VIDEO |
      SDL_INIT_GAMEPAD
   };

   Application::Application()
   {
      bool const succeeded{ SDL_InitSubSystem(INITIALIZATION_FLAGS) };
      runtime_assert(succeeded, std::format("failed to initialize SDL subsystems ({})", SDL_GetError()));

      Locator::provide<Logger>();
      Locator::provide<SystemEventDispatcher>();
      Locator::provide<InputManager>();
      Locator::provide<Window>();
      Locator::provide<Renderer>();
      Locator::provide<EditorUI>();
      Locator::provide<SceneManager>();
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