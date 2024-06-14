#include "Fronge.hpp"

#include <SDL_main.h>

#include <memory>

namespace fro
{
	std::unique_ptr<Application> createApplication();
}

int main(int, char**)
{
	fro::Logger::info("Welcome to Fronge! entering main...\n");

	fro::createApplication()->run();
	return 0;
}