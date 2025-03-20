#include "froch.hpp"
#include "render_context_event.hpp"

namespace fro
{
   std::string RenderContextCloseEvent::get_log_string() const
   {
      return std::format("closed RenderContext with ID {}", id);
   }

   std::string RenderContextResizeEvent::get_log_string() const
   {
      return std::format("resized RenderContext with ID {} to {}x{}", id, size.x, size.y);
   }
}