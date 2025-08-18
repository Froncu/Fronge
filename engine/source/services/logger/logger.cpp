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

   void Logger::log_once(Type const type, bool const engine_level, std::stacktrace_entry const location,
      std::string_view const message)
   {
      if (location and not stacktrace_entries_.insert(location).second)
         return;

      log(type, engine_level, location, message);
   }

   void Logger::log(Type const type, bool const engine_level, std::stacktrace_entry, std::string_view const message)
   {
      // TODO: remove this when MinGW works with std::println
      if constexpr (MINGW)
         std::cout <<
            std::format("[{}] [{}]: {}\n",
               engine_level ? "ENGINE" : "APP",
               type == Type::INFO ? "INFO" : type == Type::WARNING ? "WARNING" : "ERROR",
               message);
      else
         std::println("[{}] [{}]: {}",
            engine_level ? "FRONGE" : "APP",
            type == Type::INFO ? "INFO" : type == Type::WARNING ? "WARNING" : "ERROR",
            message);
   }
}