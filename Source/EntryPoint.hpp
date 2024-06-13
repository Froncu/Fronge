#if not defined ENTRY_POINT_HPP
#define ENTRY_POINT_HPP

namespace fro
{
	std::unique_ptr<Application> createApplication();
}

#if defined FRO_DISTRIBUTE
	#include "Windows.h"
	int WINAPI wWinMain(HINSTANCE const, HINSTANCE const, LPWSTR const, int const)
#else
	int main(int const, char const* const* const)
#endif
	{
		fro::createApplication()->run();
	}

#endif