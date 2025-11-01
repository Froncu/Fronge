#include <SDL3/SDL.h>

#include "fronge/application.hpp"
#include "fronge/editor_ui.hpp"
#include "fronge/entity.hpp"
#include "fronge/input_manager.hpp"
#include "fronge/locator.hpp"
#include "fronge/renderer.hpp"
#include "fronge/runtime_assert.hpp"
#include "fronge/scene_manager.hpp"
#include "fronge/system_event_dispatcher.hpp"
#include "fronge/window.hpp"

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

   Application::~Application()
   {
      Locator::remove_providers();

      SDL_QuitSubSystem(INITIALIZATION_FLAGS);
      SDL_Quit();
   }
}