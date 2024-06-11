#if not defined FRO_CORE_HPP
#define FRO_CORE_HPP

#if defined FRO_BUILD_DLL
	#define FRO_API _declspec(dllexport)
#else
	#define FRO_API _declspec(dllimport)
#endif

#endif