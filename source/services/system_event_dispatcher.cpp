#include <SDL3/SDL.h>

#include "fronge/input.hpp"
#include "fronge/locator.hpp"
#include "fronge/system_event_dispatcher.hpp"

#include "input/input_conversions.hpp"

namespace fro
{
   void SystemEventDispatcher::process_event(SDL_Event& event)
   {
      switch (event.type)
      {
         case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            window_event.notify(WindowCloseEvent{ .id{ event.window.windowID } });
            break;

         case SDL_EVENT_MOUSE_BUTTON_DOWN:
            mouse_button_event.notify(MouseButtonDownEvent{
               .button{ convert_sdl_mouse_button(event.button.button) }
            });
            break;

         case SDL_EVENT_MOUSE_BUTTON_UP:
            mouse_button_event.notify(MouseButtonUpEvent{
               .button{ convert_sdl_mouse_button(event.button.button) }
            });
            break;

         case SDL_EVENT_KEY_DOWN:
            key_event.notify(KeyDownEvent{ .key{ convert_sdl_key_code(event.key.key) } });
            break;

         case SDL_EVENT_KEY_UP:
            key_event.notify(KeyUpEvent{ .key{ convert_sdl_key_code(event.key.key) } });
            break;

         case SDL_EVENT_GAMEPAD_ADDED:
            gamepad_connection_event.notify(GamepadConnectedEvent{ .id{ event.gdevice.which } });
            break;

         case SDL_EVENT_GAMEPAD_REMOVED:
            gamepad_connection_event.notify(GamepadDisconnectedEvent{ .id{ event.gdevice.which } });
            previous_gamepad_stick_values_.erase(event.gdevice.which);
            break;

         case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            gamepad_input_event.notify(GamepadButtonDownEvent{
               .id{ event.gbutton.which },
               .button{ convert_sdl_controller_button(event.gbutton.button) }
            });
            break;

         case SDL_EVENT_GAMEPAD_BUTTON_UP:
            gamepad_input_event.notify(GamepadButtonUpEvent{
               .id{ event.gbutton.which },
               .button{ convert_sdl_controller_button(event.gbutton.button) }
            });
            break;

         case SDL_EVENT_GAMEPAD_AXIS_MOTION:
         {
            std::int16_t* previous_stick_value;
            auto& [left_stick_x, left_stick_y, right_stick_x, right_stick_y]{
               previous_gamepad_stick_values_[event.gaxis.which]
            };

            switch (event.gaxis.axis)
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
               if ((*previous_stick_value < 0 and event.gaxis.value > 0) or
                  (*previous_stick_value > 0 and event.gaxis.value < 0))
                  gamepad_input_event.notify(GamepadAxisEvent{
                     .id{ event.gaxis.which },
                     .axis{ convert_sdl_controller_axis(event.gaxis.axis, -event.gaxis.value) },
                     .value{}
                  });

               *previous_stick_value = event.gaxis.value;
            }

            auto&& [axis, value]{ convert_sdl_controller_axis_value(event.gaxis.axis, event.gaxis.value) };
            gamepad_input_event.notify(GamepadAxisEvent{
               .id{ event.gaxis.which },
               .axis{ axis },
               .value{ value }
            });
         }

         default:
            break;
      }
   }
}