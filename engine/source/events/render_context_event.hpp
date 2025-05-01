#ifndef RENDER_CONTEXT_EVENT_HPP
#define RENDER_CONTEXT_EVENT_HPP

#include "core.hpp"
#include "froch.hpp"
#include "identifier/id.hpp"
#include "maths/vector2.hpp"

namespace fro
{
   struct RenderContextCloseEvent final
   {
      ID::InternalValue const id;
   };

   struct RenderContextResizeEvent final
   {
      ID::InternalValue const id;
      Vector2<int> const size;
   };

   using RenderContextEvent = std::variant<
      RenderContextCloseEvent,
      RenderContextResizeEvent>;
}

#endif