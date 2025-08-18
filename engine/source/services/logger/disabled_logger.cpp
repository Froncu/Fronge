#include "disabled_logger.hpp"

namespace fro
{
   std::optional<std::string> DisabledLogger::format(Type, bool, std::stacktrace_entry, std::string_view)
   {
      return std::nullopt;
   }
}