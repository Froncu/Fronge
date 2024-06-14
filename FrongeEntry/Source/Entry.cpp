#include "Application/Application.hpp"

#include <SDL_main.h>

#include <memory>

namespace fro
{
	std::unique_ptr<Application> createApplication();
}

int main(int, char**)
{
	fro::createApplication()->run();
	return 0;
}