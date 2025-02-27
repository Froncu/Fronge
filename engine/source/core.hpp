#ifndef CORE_HPP
#define CORE_HPP

#define FRO_NODISCARD [[nodiscard("returned value ignored!")]]

#ifdef FRO_ENGINE
	#define FRO_API _declspec(dllexport)
#else
	#define FRO_API _declspec(dllimport)
#endif

#endif