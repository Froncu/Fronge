#include "logger.hpp"

namespace fro
{
   Logger::~Logger()
   {
      {
         std::lock_guard const lock{ mutex_ };
         run_thread_ = false;
      }

      condition_.notify_one();
   }

   std::stacktrace_entry Logger::location()
   {
      // TODO: remove this when MinGW works with std::stacktrace
      if constexpr (MINGW)
         return {};

      auto const stack_trace{ std::stacktrace::current(2, 1) };
      return stack_trace[0];
   }

   void Logger::log(Payload const& payload)
   {
      std::ostream* output_stream;
      switch (payload.type)
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

      std::time_t const now{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
      std::tm local_time;
      localtime_s(&local_time, &now);

      std::ostringstream time_stream;
      time_stream << std::put_time(&local_time, "%H:%M:%S");

      std::string_view esc_sequence;
      switch (payload.type)
      {
         case Type::INFO:
            esc_sequence = payload.engine_level ? "1;2;36;40" : "1;36;40";
            break;

         case Type::WARNING:
            esc_sequence = payload.engine_level ? "1;2;33;40" : "1;33;40";
            break;

         case Type::ERROR:
            esc_sequence = payload.engine_level ? "1;2;31;40" : "1;31;40";
            break;
      }

      // TODO: remove this when MinGW works with std::println
      if constexpr (MINGW)
         *output_stream << std::format(
            "\033[{}m[{}] {:6}: {}\033[0m\n",
            esc_sequence,
            time_stream.str(),
            payload.engine_level ? "FRONGE" : "APP",
            payload.message);
      else
         std::println(*output_stream,
            "\033[{}m[{}] {:6}: {}\033[0m",
            esc_sequence,
            time_stream.str(),
            payload.engine_level ? "FRONGE" : "APP",
            payload.message);
   }

   void Logger::log_once(Payload const& payload)
   {
      if (payload.location and not stacktrace_entries_.insert(payload.location).second)
         return;

      return log(payload);
   }
}