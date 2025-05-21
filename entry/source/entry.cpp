#include <SDL3/SDL_main.h>

#include "fronge.hpp"

namespace fro
{
   std::unique_ptr<Application> create_application();
}

int main(int, char**)
{
   fro::create_application()->run();
   fro::Locator::reset();
   return 0;
}