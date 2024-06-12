#if not defined CORE_HPP
#define CORE_HPP

#if defined FRO_BUILD_DLL
	#define FRO_API _declspec(dllexport)
#else
	#define FRO_API _declspec(dllimport)
#endif

#endif