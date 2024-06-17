#if not defined CORE_HPP
#define CORE_HPP

#define FRO_NODISCARD [[nodiscard("returned value ignored!")]]



#if defined FRO_BUILD_DLL
	#define FRO_API _declspec(dllexport)
#else
	#define FRO_API _declspec(dllimport)
#endif



#if defined FRO_DEBUG
	#include "Logger.hpp"
	#define FRO_ASSERT(condition, ...) { if (not (condition)) { fro::Logger::error(__VA_ARGS__); __debugbreak(); } }
#else
	#define FRO_ASSERT(condition, ...)
#endif

#endif