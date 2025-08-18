#ifndef DISABLED_LOGGER_HPP
#define DISABLED_LOGGER_HPP

#include "logger.hpp"

namespace fro
{
   class DisabledLogger final : public Logger
   {
      public:
         DisabledLogger() = default;
         DisabledLogger(DisabledLogger const&) = delete;
         DisabledLogger(DisabledLogger&&) = default;

         virtual ~DisabledLogger() override = default;

         DisabledLogger& operator=(DisabledLogger const&) = delete;
         DisabledLogger& operator=(DisabledLogger&&) = default;

      private:
         FRO_API [[nodiscard]] virtual std::optional<std::string> format(Type type, bool engine_level,
            std::stacktrace_entry location, std::string_view message) override;
   };
}

#endif