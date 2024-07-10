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

#if defined FRO_DEBUG
	Logger::separator();
#endif

	Logger::info("welcome to Fronge!\n");

	Audio::initialize();

	createApplication()->run();

	Audio::shutDown();

	Logger::separator();

	return 0;
}