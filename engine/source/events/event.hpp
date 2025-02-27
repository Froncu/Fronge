#ifndef EVENT_HPP
#define EVENT_HPP

#include "froch.hpp"

namespace fro
{
   template <typename T>
   concept Event = requires(T event)
   {
      { event.get_log_string() } -> std::same_as<std::string>;
   };

   template <Event... Ts>
   using Events = std::variant<Ts...>;
}

#endif