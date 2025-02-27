#include "logger/logger.hpp"
#include "system_event_dispatcher.hpp"

namespace fro
{
   void SystemEventDispatcher::poll_events()
   {
      Logger::error("no provider set for SystemEventDispatcherService!");
   }
}