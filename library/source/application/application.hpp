#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "core.hpp"

namespace fro
{
   class Application
   {
      public:
         FRO_API Application();
         FRO_API Application(Application const&);
         FRO_API Application(Application&&) noexcept;

         FRO_API virtual ~Application();

         Application& operator=(Application const&) = default;
         Application& operator=(Application&&) = default;

         virtual void run() = 0;
   };
}

#endif