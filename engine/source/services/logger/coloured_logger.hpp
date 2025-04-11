#ifndef COLOURED_LOGGER_HPP
#define COLOURED_LOGGER_HPP

#include "logger.hpp"

namespace fro
{
   class ColouredLogger final : public Logger
   {
      public:
         ColouredLogger() = default;
         ColouredLogger(ColouredLogger const&) = default;
         ColouredLogger(ColouredLogger&&) = default;

         virtual ~ColouredLogger() override = default;

         ColouredLogger& operator=(ColouredLogger const&) = default;
         ColouredLogger& operator=(ColouredLogger&&) = default;

      private:
         FRO_API virtual void log(Type type, bool engine_level, std::string_view message) override;
   };
}

#endif