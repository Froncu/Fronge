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
            log(Type::INFO, ENGINE, std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename... Arguments>
         void warning(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log(Type::WARNING, ENGINE, std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename... Arguments>
         void error(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            log(Type::ERROR, ENGINE, std::format(format, std::forward<Arguments>(arguments)...));
         }

         template <typename Message>
         void info(Message&& value)
         {
            info("{}", std::forward<Message>(value));
         }

         template <typename Message>
         void warning(Message&& value)
         {
            warning("{}", std::forward<Message>(value));
         }

         template <typename Message>
         void error(Message&& value)
         {
            error("{}", std::forward<Message>(value));
         }

      private:
         FRO_API virtual void log(Type type, bool engine_level, std::string_view message);
   };
}

#endif