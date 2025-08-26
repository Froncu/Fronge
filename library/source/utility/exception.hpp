#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "services/locator.hpp"
#include "services/logger/logger.hpp"

namespace fro
{
   template <std::stacktrace::size_type StackTraceDepth = 0, typename... Arguments>
   [[noreturn]] void exception(std::format_string<Arguments...> const format,
      Arguments&&... arguments)
   {
      std::string const message{ std::format(format, std::forward<Arguments>(arguments)...) };
      Locator::get<Logger>().error<StackTraceDepth + 1>(message);
      throw std::runtime_error(message);
   }

   template <typename Message>
   [[noreturn]] void exception(Message&& message)
   {
      exception<1>("{}", std::forward<Message>(message));
   }
}

#endif