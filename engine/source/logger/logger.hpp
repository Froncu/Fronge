#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "constants.hpp"
#include "core.hpp"
#include "froch.hpp"

namespace fro
{
   class Logger final
   {
      public:
         static std::string_view constexpr LOGGER_NAME{ ENGINE ? "FRONGE" : "APP" };

         template <typename... Arguments>
         static constexpr void info(std::format_string<Arguments...> const format,
            Arguments&&... arguments)
         {
            log(ENGINE ? "1;2;36;40" : "1;36;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
         }

         template <typename... Arguments>
         static constexpr void warn(std::format_string<Arguments...> const format,
            Arguments&&... arguments)
         {
            log(ENGINE ? "1;2;33;40" : "1;33;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
         }

         template <typename... Arguments>
         static constexpr void error(std::format_string<Arguments...> const format,
            Arguments&&... arguments)
         {
            log(ENGINE ? "1;2;31;40" : "1;31;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
         }

         template <typename Type>
         static constexpr void info(Type&& value)
         {
            info("{}", std::forward<Type>(value));
         }

         template <typename Type>
         static constexpr void warn(Type&& value)
         {
            warn("{}", std::forward<Type>(value));
         }

         template <typename Type>
         static constexpr void error(Type&& value)
         {
            error("{}", std::forward<Type>(value));
         }

         Logger() = delete;
         Logger(Logger const&) = delete;
         Logger(Logger&&) = delete;

         ~Logger() = delete;

         Logger& operator=(Logger const&) = delete;
         Logger& operator=(Logger&&) = delete;

      private:
         template <typename... Arguments>
         static constexpr void log([[maybe_unused]] std::string_view const esc_sequence,
            [[maybe_unused]] std::string_view const logger_name,
            [[maybe_unused]] std::format_string<Arguments...> const format, [[maybe_unused]] Arguments&&... arguments)
         {
            if constexpr (LOGGER)
            {
               auto const now{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
               std::tm local_time;
               localtime_s(&local_time, &now);

               std::ostringstream time_stream;
               time_stream << std::put_time(&local_time, "%H:%M:%S");

               std::lock_guard const lock{ output_mutex_ };
               std::cout
                  << std::format("\033[{}m", esc_sequence)
                  << std::format("[{}] {:6}: ", time_stream.str(), logger_name)
                  << std::format(format, std::forward<Arguments>(arguments)...)
                  << "\033[0m\n";
            }
         }

         FRO_API static std::mutex output_mutex_;
   };
}

#endif