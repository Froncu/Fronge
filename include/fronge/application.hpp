#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "fronge/api.hpp"

namespace fro
{
   class Application
   {
      public:
         Application(Application const&) = delete;
         Application(Application&&) noexcept = delete;

         FRO_API virtual ~Application();

         Application& operator=(Application const&) = default;
         Application& operator=(Application&&) = default;

         virtual void run() = 0;

      protected:
         FRO_API Application();
   };
}

#endif