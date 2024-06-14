#include "Application/Application.hpp"

#include <memory>

namespace fro
{
	std::unique_ptr<Application> createApplication();
}

#if defined FRO_DISTRIBUTE
#include <Windows.h>
int WINAPI WinMain(HINSTANCE const, HINSTANCE const, LPSTR const, int const)
#else
int main(int, char**)
#endif
{
	fro::createApplication()->run();
	return 0;
}