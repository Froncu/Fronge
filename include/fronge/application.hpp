#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "fronge/api.hpp"

union SDL_Event;

namespace fro
{
   class Application
   {
      public:
         Application(Application const&) = delete;
         Application(Application&&) noexcept = delete;

         FRO_API virtual ~Application();

         Application& operator=(Application const&) = delete;
         Application& operator=(Application&&) = delete;

         [[nodiscard]] virtual bool tick() = 0;
         FRO_API void process_event(SDL_Event& event);

      protected:
         FRO_API Application();
   };
}

#endif