#include "froch.hpp"
#include "window_event.hpp"

namespace fro
{
   std::string WindowCloseEvent::get_log_string() const
   {
      return std::format("closed Window with ID {}", id);
   }

   std::string WindowResizeEvent::get_log_string() const
   {
      return std::format("resized Window with ID {} to {}x{}", id, size.x, size.y);
   }
}