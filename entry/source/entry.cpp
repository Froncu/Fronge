#include "Fronge.hpp"

#include <SDL_main.h>

#include <memory>

namespace fro
{
   std::unique_ptr<Application> createApplication();
}

int main(int, char**)
{
   using namespace fro;

   Logger::info("welcome to Fronge!\n");

   createApplication()->run();

   scene_manager::shutDown();

   return 0;
}