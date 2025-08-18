#include "logger.hpp"

namespace fro
{
   std::stacktrace_entry Logger::location()
   {
      // TODO: remove this when MinGW works with std::stacktrace
      if constexpr (MINGW)
         return {};

      auto const stack_trace{ std::stacktrace::current(2, 1) };
      return stack_trace[0];
   }

   void Logger::log(Type const type, bool const engine_level, std::stacktrace_entry const location,
      std::string_view const message)
   {
      std::optional const formatted_message{ format(type, engine_level, location, message) };
      if (not formatted_message)
         return;

      std::ostream* output_stream;
      switch (type)
      {
         case Type::INFO:
            output_stream = &std::clog;
            break;

         case Type::WARNING:
            [[fallthrough]];

         case Type::ERROR:
            output_stream = &std::cerr;
            break;

         default:
            output_stream = &std::cout;
      }

      // TODO: remove this when MinGW works with std::println
      if constexpr (MINGW)
         *output_stream << *formatted_message << '\n';
      else
         std::println(*output_stream, "{}", *formatted_message);
   }

   void Logger::log_once(Type const type, bool const engine_level, std::stacktrace_entry const location,
      std::string_view const message)
   {
      if (location and not stacktrace_entries_.insert(location).second)
         return;

      return log(type, engine_level, location, message);
   }

   std::optional<std::string> Logger::format(Type const type, bool const engine_level, std::stacktrace_entry,
      std::string_view const message)
   {
      return std::format("[{}] [{}]: {}\n",
         engine_level ? "ENGINE" : "APP",
         type == Type::INFO ? "INFO" : type == Type::WARNING ? "WARNING" : "ERROR",
         message);
   }
}