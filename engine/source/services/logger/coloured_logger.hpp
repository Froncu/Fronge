#ifndef COLOURED_LOGGER_HPP
#define COLOURED_LOGGER_HPP

#include "logger.hpp"

namespace fro
{
   class ColouredLogger final : public Logger
   {
      public:
         ColouredLogger() = default;
         ColouredLogger(ColouredLogger const&) = delete;
         ColouredLogger(ColouredLogger&&) = default;

         virtual ~ColouredLogger() override = default;

         ColouredLogger& operator=(ColouredLogger const&) = delete;
         ColouredLogger& operator=(ColouredLogger&&) = default;

      private:
         FRO_API [[nodiscard]] virtual std::optional<std::string> format(Type type, bool engine_level,
            std::stacktrace_entry location, std::string_view message) override;
   };
}

#endif