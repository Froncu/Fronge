#if not defined FRO_ENTRY_POINT
#define FRO_ENTRY_POINT

#if defined FRO_PLATFORM_WINDOWS

extern std::unique_ptr<fro::Application> fro::createApplication();

int main(int, char**)
{
	fro::createApplication()->run();
}

#endif

#endif