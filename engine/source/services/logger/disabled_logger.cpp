#include "disabled_logger.hpp"

namespace fro
{
   void DisabledLogger::log(Type, bool, std::stacktrace_entry, std::string_view)
   {
   }
}