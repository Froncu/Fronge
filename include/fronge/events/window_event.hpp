#ifndef WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "../pch.hpp"
#include "../identifier/id.hpp"
#include "../maths/vector2.hpp"

namespace fro
{
   struct WindowCloseEvent final
   {
      ID::InternalValue const id;
   };

   struct WindowResizeEvent final
   {
      ID::InternalValue const id;
      Vector2<int> const size;
   };

   using WindowEvent = std::variant<
      WindowCloseEvent,
      WindowResizeEvent>;
}

#endif