#ifndef WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "core.hpp"
#include "event.hpp"
#include "froch.hpp"
#include "maths/vector2.hpp"

namespace fro
{
   struct WindowCloseEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      std::uint32_t const id;
   };

   struct WindowResizeEvent final
   {
      FRO_API [[nodiscard]] std::string get_log_string() const;

      std::uint32_t const id;
      Vector2<int> const size;
   };

   using WindowEvent = Events<
      WindowCloseEvent,
      WindowResizeEvent>;
}

#endif