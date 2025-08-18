#include "coloured_logger.hpp"

namespace fro
{
   void ColouredLogger::log(Type const type, bool const engine_level, std::stacktrace_entry, std::string_view const message)
   {
      std::time_t const now{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
      std::tm local_time;
      localtime_s(&local_time, &now);

      std::ostringstream time_stream;
      time_stream << std::put_time(&local_time, "%H:%M:%S");

      std::string_view esc_sequence;
      switch (type)
      {
         case Type::INFO:
            esc_sequence = engine_level ? "1;2;36;40" : "1;36;40";
            break;

         case Type::WARNING:
            esc_sequence = engine_level ? "1;2;33;40" : "1;33;40";
            break;

         case Type::ERROR:
            esc_sequence = engine_level ? "1;2;31;40" : "1;31;40";
            break;
      }

      // TODO: remove this when MinGW works with std::println
      if constexpr (MINGW)
         std::cout
            << std::format("\033[{}m", esc_sequence)
            << std::format("[{}] {:6}: ", time_stream.str(), engine_level ? "FRONGE" : "APP")
            << message
            << "\033[0m\n";
      else
         std::println("\033[{}m[{}] {:6}: {}\033[0m",
            esc_sequence,
            time_stream.str(),
            engine_level ? "FRONGE" : "APP",
            message
         );
   }
}