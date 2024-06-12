#if not defined ENTRY_POINT_HPP
#define ENTRY_POINT_HPP

namespace fro
{
	std::unique_ptr<Application> createApplication();
}

int main(int, char**)
{
	fro::createApplication()->run();
}

#endif