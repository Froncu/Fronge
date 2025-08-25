#ifndef SYTEM_EVENT_DISPATCHER_HPP
#define SYTEM_EVENT_DISPATCHER_HPP

#include "core.hpp"
#include "events/input_events.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "events/render_context_event.hpp"

union SDL_Event;

namespace fro
{
   class SystemEventDispatcher final
   {
      struct GamepadStickValues final
      {
         std::int16_t left_stick_x{};
         std::int16_t left_stick_y{};
         std::int16_t right_stick_x{};
         std::int16_t right_stick_y{};
      };

      public:
         SystemEventDispatcher() = default;
         SystemEventDispatcher(SystemEventDispatcher const&) = delete;
         SystemEventDispatcher(SystemEventDispatcher&&) = default;

         ~SystemEventDispatcher() = default;

         SystemEventDispatcher& operator=(SystemEventDispatcher const&) = delete;
         SystemEventDispatcher& operator=(SystemEventDispatcher&&) = default;

         FRO_API void poll_events();

         EventDispatcher<RenderContextEvent const> render_context_event{};
         EventDispatcher<MouseButtonEvent const> mouse_button_event{};
         EventDispatcher<KeyEvent const> key_event{};
         EventDispatcher<GamepadConnectionEvent const> gamepad_connection_event{};
         EventDispatcher<GamepadInputEvent const> gamepad_input_event{};
         EventDispatcher<SDL_Event const> native_event{};

      private:
         std::unordered_map<std::uint32_t, GamepadStickValues> previous_gamepad_stick_values_{};
   };
}

#endif