#ifndef APPLICATION_HPP
#define APPLICATION_HPP

namespace fro
{
   class Application
   {
      public:
         Application() = default;
         Application(Application const&) = default;
         Application(Application&&) = default;

         virtual ~Application() = default;

         Application& operator=(Application const&) = default;
         Application& operator=(Application&&) = default;

         virtual void run() = 0;
   };
}

#endif