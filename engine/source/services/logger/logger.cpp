#include "logger.hpp"

namespace fro
{
   void Logger::log(Type, bool, std::string_view)
   {
      std::cout << "no provider set for Logger!\n";
   }
}