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
         FRO_API RegularSystemEventManager();
         RegularSystemEventManager(RegularSystemEventManager const&) = default;
         RegularSystemEventManager(RegularSystemEventManager&&) = default;

         FRO_API virtual ~RegularSystemEventManager() override;

         RegularSystemEventManager& operator=(RegularSystemEventManager const&) = default;
         RegularSystemEventManager& operator=(RegularSystemEventManager&&) = default;

         FRO_API virtual void poll_events() override;

      private:
         std::unordered_map<std::uint32_t, GamepadStickValues> previous_gamepad_stick_values_{};
   };
}

#endif