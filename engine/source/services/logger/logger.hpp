#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "constants.hpp"
#include "core.hpp"
#include "froch.hpp"

namespace fro
{
   class Logger
   {
      protected:
         enum class Type
         {
            INFO,
            WARNING,
            ERROR
         };

      public:
         Logger() = default;
         Logger(Logger const&) = default;
         Logger(Logger&&) = default;

         virtual ~Logger() = default;

         Logger& operator=(Logger const&) = default;
         Logger& operator=(Logger&&) = default;

         template <typename... Arguments>
         void info(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log(Type::INFO, ENGINE, location(), std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename Message>
         void info(Message&& message)
         {
            log(Type::INFO, ENGINE, location(), std::format("{}", std::forward<Message>(message)));
         }

         template <typename... Arguments>
         void info_once(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log_once(Type::INFO, ENGINE, location(), std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename Message>
         void info_once(Message&& message)
         {
            log_once(Type::INFO, ENGINE, location(), std::format("{}", std::forward<Message>(message)));
         }

         template <typename... Arguments>
         void warning(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log(Type::WARNING, ENGINE, location(), std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename Message>
         void warning(Message&& message)
         {
            log(Type::WARNING, ENGINE, location(), std::format("{}", std::forward<Message>(message)));
         }

         template <typename... Arguments>
         void warning_once(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log_once(Type::WARNING, ENGINE, location(), std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename Message>
         void warning_once(Message&& message)
         {
            log_once(Type::WARNING, ENGINE, location(), std::format("{}", std::forward<Message>(message)));
         }

         template <typename... Arguments>
         void error(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log(Type::ERROR, ENGINE, location(), std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename Message>
         void error(Message&& message)
         {
            log(Type::ERROR, ENGINE, location(), std::format("{}", std::forward<Message>(message)));
         }

         template <typename... Arguments>
         void error_once(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log_once(Type::ERROR, ENGINE, location(), std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename Message>
         void error_once(Message&& message)
         {
            log_once(Type::ERROR, ENGINE, location(), std::format("{}", std::forward<Message>(message)));
         }

      private:
         FRO_API [[nodiscard]] static std::stacktrace_entry location();

         FRO_API void log_once(Type type, bool engine_level, std::stacktrace_entry location, std::string_view message);
         FRO_API virtual void log(Type type, bool engine_level, std::stacktrace_entry location, std::string_view message);

         std::unordered_set<std::stacktrace_entry> stacktrace_entries_{};
   };
}

#endif