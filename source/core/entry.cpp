#include <SDL3/SDL_main.h>

#include "fronge/fronge.hpp"

namespace fro
{
   [[nodiscard]] std::unique_ptr<Application> create_application();
}

int main(int, char**)
{
   fro::create_application()->run();
   return 0;
}