#ifndef CORE_HPP
#define CORE_HPP

#define FRO_NODISCARD [[nodiscard("returned value ignored!")]]

//---

#ifdef FRO_BUILD_DLL
	#define FRO_API _declspec(dllexport)
#else
	#define FRO_API _declspec(dllimport)
#endif

//---

#include "Logger/Logger.hpp"

#ifdef FRO_DEBUG
	#define FRO_ASSERT(condition, ...) { if (not (condition)) { fro::Logger::error(__VA_ARGS__); __debugbreak(); } }
#else
	#define FRO_ASSERT(condition, ...)
#endif

#define FRO_EXCEPTION(...) { fro::Logger::error(__VA_ARGS__); throw std::runtime_error(std::format(__VA_ARGS__)); }

#endif