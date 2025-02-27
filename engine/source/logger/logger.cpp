#include "logger.hpp"

namespace fro
{
   std::mutex Logger::output_mutex_{};
}