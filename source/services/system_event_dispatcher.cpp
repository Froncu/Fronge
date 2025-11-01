#include <SDL3/SDL.h>

#include "fronge/editor_ui.hpp"
#include "fronge/input.hpp"
#include "fronge/locator.hpp"
#include "fronge/system_event_dispatcher.hpp"

#include "input/input_conversions.hpp"

namespace fro
{
   void SystemEventDispatcher::poll_events()
   {
      SDL_Event polled_event;
      while (SDL_PollEvent(&polled_event))
      {
         native_event.notify(polled_event);

         auto const& editor_ui{ Locator::get<EditorUI>() }; // TODO: this should not be used here
         switch (polled_event.type)
         {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
               window_event.notify(WindowCloseEvent{ .id{ polled_event.window.windowID } });
               break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
               if (not editor_ui.captures_mouse())
                  mouse_button_event.notify(MouseButtonDownEvent{
                     .button{ convert_sdl_mouse_button(polled_event.button.button) }
                  });
               break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
               if (not editor_ui.captures_mouse())
                  mouse_button_event.notify(MouseButtonUpEvent{
                     .button{ convert_sdl_mouse_button(polled_event.button.button) }
                  });
               break;

            case SDL_EVENT_KEY_DOWN:
               if (not editor_ui.captures_keyboard())
                  key_event.notify(KeyDownEvent{ .key{ convert_sdl_key_code(polled_event.key.key) } });
               break;

            case SDL_EVENT_KEY_UP:
               if (not editor_ui.captures_keyboard())
                  key_event.notify(KeyUpEvent{ .key{ convert_sdl_key_code(polled_event.key.key) } });
               break;

            case SDL_EVENT_GAMEPAD_ADDED:
               gamepad_connection_event.notify(GamepadConnectedEvent{ .id{ polled_event.gdevice.which } });
               break;

            case SDL_EVENT_GAMEPAD_REMOVED:
               gamepad_connection_event.notify(GamepadDisconnectedEvent{ .id{ polled_event.gdevice.which } });
               previous_gamepad_stick_values_.erase(polled_event.gdevice.which);
               break;

            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
               gamepad_input_event.notify(GamepadButtonDownEvent{
                  .id{ polled_event.gbutton.which },
                  .button{ convert_sdl_controller_button(polled_event.gbutton.button) }
               });
               break;

            case SDL_EVENT_GAMEPAD_BUTTON_UP:
               gamepad_input_event.notify(GamepadButtonUpEvent{
                  .id{ polled_event.gbutton.which },
                  .button{ convert_sdl_controller_button(polled_event.gbutton.button) }
               });
               break;

            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            {
               std::int16_t* previous_stick_value;
               auto& [left_stick_x, left_stick_y, right_stick_x, right_stick_y]{
                  previous_gamepad_stick_values_[polled_event.gaxis.which]
               };

               switch (polled_event.gaxis.axis)
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
                  if ((*previous_stick_value < 0 and polled_event.gaxis.value > 0) or
                     (*previous_stick_value > 0 and polled_event.gaxis.value < 0))
                     gamepad_input_event.notify(GamepadAxisEvent{
                        .id{ polled_event.gaxis.which },
                        .axis{ convert_sdl_controller_axis(polled_event.gaxis.axis, -polled_event.gaxis.value) },
                        .value{}
                     });

                  *previous_stick_value = polled_event.gaxis.value;
               }

               auto&& [axis, value]{ convert_sdl_controller_axis_value(polled_event.gaxis.axis, polled_event.gaxis.value) };
               gamepad_input_event.notify(GamepadAxisEvent{
                  .id{ polled_event.gaxis.which },
                  .axis{ axis },
                  .value{ value }
               });
            }

            default:
               break;
         }
      }
   }
}