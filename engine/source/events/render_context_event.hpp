#ifndef RENDER_CONTEXT_EVENT_HPP
#define RENDER_CONTEXT_EVENT_HPP

#include "core.hpp"
#include "event.hpp"
#include "froch.hpp"
#include "maths/vector2.hpp"

namespace fro
{
   struct RenderContextCloseEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      std::uint32_t const id;
   };

   struct RenderContextResizeEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      std::uint32_t const id;
      Vector2<int> const size;
   };

   using RenderContextEvent = Events<
      RenderContextCloseEvent,
      RenderContextResizeEvent>;
}

#endif