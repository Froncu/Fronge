#include "services/logger/logger.hpp"
#include "system_event_dispatcher.hpp"

namespace fro
{
   void SystemEventDispatcher::poll_events()
   {
      Locator::get<Logger>().error("no provider set for SystemEventDispatcher!");
   }
}