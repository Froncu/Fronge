#ifndef RESGULAR_SYTEM_EVENT_DISPATCHER_HPP
#define RESGULAR_SYTEM_EVENT_DISPATCHER_HPP

#include "core.hpp"
#include "system_event_dispatcher.hpp"

namespace fro
{
   class RegularSystemEventManager final : public SystemEventDispatcher
   {
      struct GamepadStickValues final
      {
         std::int16_t left_stick_x{};
         std::int16_t left_stick_y{};
         std::int16_t right_stick_x{};
         std::int16_t right_stick_y{};
      };

      public:
         RegularSystemEventManager() = default;
         RegularSystemEventManager(RegularSystemEventManager const&) = default;
         RegularSystemEventManager(RegularSystemEventManager&&) = default;

         virtual ~RegularSystemEventManager() override = default;

         RegularSystemEventManager& operator=(RegularSystemEventManager const&) = default;
         RegularSystemEventManager& operator=(RegularSystemEventManager&&) = default;

         FRO_API virtual void poll_events() override;

      private:
         std::unordered_map<std::uint32_t, GamepadStickValues> previous_gamepad_stick_values_{};
   };
}

#endif