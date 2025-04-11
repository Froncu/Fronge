#include "froch.hpp"
#include "input_event.hpp"

namespace fro
{
   std::string MouseButtonDownEvent::get_log_string() const
   {
      return "mouse button down event";
   }

   std::string MouseButtonUpEvent::get_log_string() const
   {
      return "mouse button up event";
   }

   std::string KeyDownEvent::get_log_string() const
   {
      return "key down event";
   }

   std::string KeyUpEvent::get_log_string() const
   {
      return "key up event";
   }

   std::string GamepadConnectedEvent::get_log_string() const
   {
      return std::format("gamepad with device ID {} connected event",
         id);
   }

   std::string GamepadDisconnectedEvent::get_log_string() const
   {
      return std::format("Gamepad with ID {} disconnected event",
         id);
   }

   std::string GamepadButtonDownEvent::get_log_string() const
   {
      return std::format("Gamepad with ID {} button down event",
         id);
   }

   std::string GamepadButtonUpEvent::get_log_string() const
   {
      return std::format("Gamepad with ID {} button up event",
         id);
   }

   std::string GamepadAxisEvent::get_log_string() const
   {
      return std::format("Gamepad with ID {} axis event",
         id);
   }
}