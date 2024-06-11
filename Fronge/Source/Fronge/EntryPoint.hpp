#if not defined FRO_ENTRY_POINT_HPP
#define FRO_ENTRY_POINT_HPP

extern std::unique_ptr<fro::Application> fro::createApplication();

int main(int, char**)
{
	fro::createApplication()->run();
}

#endif