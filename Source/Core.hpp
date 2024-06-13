#if not defined CORE_HPP
#define CORE_HPP

#if defined FRO_BUILD_DLL
	#define FRO_API _declspec(dllexport)
#else
	#define FRO_API _declspec(dllimport)
#endif



#if defined FRO_DEBUG
	#include "Logger.hpp"
	#define FRO_ASSERT(x, ...) { if (not (x)) { fro::Logger::error("assertion failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
	#define FRO_ASSERT(x, ...)
#endif

#endif