#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "services/locator.hpp"
#include "services/logger/logger.hpp"

namespace fro
{
   template <typename... Arguments>
   [[noreturn]] void exception(std::format_string<Arguments...> const format,
      Arguments&&... arguments)
   {
      std::string const message{ std::format(format, std::forward<Arguments>(arguments)...) };
      Locator::get<Logger>().error(message);
      throw std::runtime_error(message);
   }

   template <typename Message>
   [[noreturn]] void exception(Message&& message)
   {
      exception("{}", std::forward<Message>(message));
   }
}

#endif