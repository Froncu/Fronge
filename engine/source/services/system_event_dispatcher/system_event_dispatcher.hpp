#ifndef SYTEM_EVENT_DISPATCHER_HPP
#define SYTEM_EVENT_DISPATCHER_HPP

#include "core.hpp"
#include "events/input_events.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "events/render_context_event.hpp"

namespace fro
{
   class SystemEventDispatcher
   {
      public:
         SystemEventDispatcher() = default;
         SystemEventDispatcher(SystemEventDispatcher const&) = default;
         SystemEventDispatcher(SystemEventDispatcher&&) = default;

         virtual ~SystemEventDispatcher() = default;

         SystemEventDispatcher& operator=(SystemEventDispatcher const&) = default;
         SystemEventDispatcher& operator=(SystemEventDispatcher&&) = default;

         FRO_API virtual void poll_events();

         EventDispatcher<RenderContextEvent const> render_context_event{};
         EventDispatcher<MouseButtonEvent const> mouse_button_event{};
         EventDispatcher<KeyEvent const> key_event{};
         EventDispatcher<GamepadConnectionEvent const> gamepad_connection_event{};
         EventDispatcher<GamepadInputEvent const> gamepad_input_event{};
   };
}

#endif