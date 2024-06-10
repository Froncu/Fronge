#if not defined FRO_CORE_H
#define FRO_CORE_H

#if defined FRO_PLATFORM_WINDOWS
	#if defined FRO_BUILD_DLL
		#define FRO_API _declspec(dllexport)
	#else
		#define FRO_API _declspec(dllimport)
	#endif
#else
	#error Fronge only supports Windows!
#endif

#endif