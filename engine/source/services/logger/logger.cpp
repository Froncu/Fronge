#include "logger.hpp"

namespace fro
{
   void Logger::log(Type const type, bool const engine_level, std::string_view const message)
   {
      std::cout <<
         std::format("[{}] [{}]: {}\n",
            engine_level ? "ENGINE" : "APP",
            type == Type::INFO ? "INFO" : type == Type::WARNING ? "WARNING" : "ERROR",
            message);
   }
}