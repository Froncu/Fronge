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
               previous_gamepad_stick_values_.erase(native_event.gdevice.which);
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
               std::int16_t* previous_stick_value;
               auto& [left_stick_x, left_stick_y, right_stick_x, right_stick_y]{
                  previous_gamepad_stick_values_[native_event.gaxis.which]
               };

               switch (native_event.gaxis.axis)
               {
                  case SDL_GAMEPAD_AXIS_LEFTX:
                     previous_stick_value = &left_stick_x;
                     break;

                  case SDL_GAMEPAD_AXIS_LEFTY:
                     previous_stick_value = &left_stick_y;
                     break;

                  case SDL_GAMEPAD_AXIS_RIGHTX:
                     previous_stick_value = &right_stick_x;
                     break;

                  case SDL_GAMEPAD_AXIS_RIGHTY:
                     previous_stick_value = &right_stick_y;
                     break;

                  default:
                     previous_stick_value = nullptr;
                     break;
               }

               if (previous_stick_value)
               {
                  if ((*previous_stick_value < 0 and native_event.gaxis.value > 0) or
                     (*previous_stick_value > 0 and native_event.gaxis.value < 0))
                  {
                     GamepadAxis const opposite_axis{
                        convert_sdl_controller_axis(native_event.gaxis.axis, -native_event.gaxis.value)
                     };

                     input_event.notify(GamepadAxisEvent{
                        .input{ native_event.gaxis.which, opposite_axis },
                        .value{}
                     });
                  }

                  *previous_stick_value = native_event.gaxis.value;
               };

               auto&& [axis, value]{ convert_sdl_controller_axis_value(native_event.gaxis.axis, native_event.gaxis.value) };
               input_event.notify(GamepadAxisEvent{
                  .input{ native_event.gaxis.which, axis },
                  .value{ value }
               });
            }

            default:
               break;
         }
   }
}