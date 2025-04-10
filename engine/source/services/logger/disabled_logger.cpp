#include "disabled_logger.hpp"

namespace fro
{
   void DisabledLogger::log(Type, bool, std::string_view)
   {
   }
}