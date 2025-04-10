#include <SDL3/SDL.h>

#include "input/input.hpp"
#include "input/input_conversions.hpp"
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
               render_context_event.notify(RenderContextCloseEvent{ .id{ native_event.window.windowID } });
               break;

            case SDL_EVENT_KEY_DOWN:
               input_event.notify(KeyDownEvent{ .key{ convert_sdl_key_code(native_event.key.key) } });
               break;

            case SDL_EVENT_KEY_UP:
               input_event.notify(KeyUpEvent{ .key{ convert_sdl_key_code(native_event.key.key) } });
               break;

            case SDL_EVENT_GAMEPAD_ADDED:
               input_event.notify(GamepadConnectedEvent{ .id{ native_event.gdevice.which } });
               break;

            case SDL_EVENT_GAMEPAD_REMOVED:
               input_event.notify(GamepadDisconnectedEvent{ .id{ native_event.gdevice.which } });
               break;

            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
               input_event.notify(GamepadButtonDownEvent{
                  .input{ native_event.gbutton.which, convert_sdl_controller_button(native_event.gbutton.button) }
               });
               break;

            case SDL_EVENT_GAMEPAD_BUTTON_UP:
               input_event.notify(GamepadButtonUpEvent{
                  .input{ native_event.gbutton.which, convert_sdl_controller_button(native_event.gbutton.button) }
               });
               break;

            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            {
               auto&& [axis, value]{ convert_sdl_controller_axis(native_event.gaxis.axis, native_event.gaxis.value) };
               input_event.notify(GamepadAxisEvent{
                  .input{ native_event.gaxis.which, axis },
                  .value{ value }
               });
               break;
            }

            default:
               break;
         }
   }
}