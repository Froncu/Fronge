#ifndef SYTEM_EVENT_MANAGER_HPP
#define SYTEM_EVENT_MANAGER_HPP

#include "core.hpp"
#include "system_event_dispatcher.hpp"

namespace fro
{
   class RegularSystemEventManager final : public SystemEventDispatcher
   {
      public:
         FRO_API RegularSystemEventManager();
         RegularSystemEventManager(RegularSystemEventManager const&) = default;
         RegularSystemEventManager(RegularSystemEventManager&&) = default;

         FRO_API virtual ~RegularSystemEventManager() override;

         RegularSystemEventManager& operator=(RegularSystemEventManager const&) = default;
         RegularSystemEventManager& operator=(RegularSystemEventManager&&) = default;

         FRO_API virtual void poll_events() override;
   };
}

#endif