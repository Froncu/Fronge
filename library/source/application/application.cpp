#include <SDL3/SDL.h>

#include "application.hpp"
#include "ecs/entity.hpp" // TODO: remove when the file structure for ECS is improved
#include "services/editor_ui/editor_ui.hpp"
#include "services/input_manager/input_manager.hpp"
#include "services/locator.hpp"
#include "services/renderer/renderer.hpp"
#include "services/scene_manager/scene_manager.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "services/window/window.hpp"
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
      bool const succeeded{ SDL_InitSubSystem(INITIALIZATION_FLAGS) };
      runtime_assert(succeeded, "failed to initialize SDL subsystems ({})",
         SDL_GetError());

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