#include <SDL3/SDL_main.h>

#include "fronge.hpp"

namespace fro
{
   std::unique_ptr<Application> create_application();
}

int main(int, char**)
{
   fro::Logger::info("welcome to Fronge!\n");

   fro::create_application()->run();

   return 0;
}