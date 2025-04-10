#ifndef INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "core.hpp"
#include "event.hpp"
#include "froch.hpp"
#include "input/input.hpp"

namespace fro
{
   struct MouseButtonDownEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      MouseButton const button;
   };

   struct MouseButtonUpEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      MouseButton const button;
   };

   struct KeyDownEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      Key const key;
   };

   struct KeyUpEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      Key const key;
   };

   struct GamepadConnectedEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      std::uint32_t const id;
   };

   struct GamepadDisconnectedEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      std::uint32_t const id;
   };

   struct GamepadButtonDownEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      GamepadButtonInput const input;
   };

   struct GamepadButtonUpEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      GamepadButtonInput const input;
   };

   struct GamepadAxisEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      GamepadAxisInput const input;
      double const value;
   };

   using InputEvent = Events<
      MouseButtonDownEvent,
      MouseButtonUpEvent,
      KeyDownEvent,
      KeyUpEvent,
      GamepadConnectedEvent,
      GamepadDisconnectedEvent,
      GamepadButtonDownEvent,
      GamepadButtonUpEvent,
      GamepadAxisEvent>;
}

#endif