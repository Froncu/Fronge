#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "Logger/Logger.hpp"

namespace fro
{
	template <typename... Arguments>
	[[noreturn]] void exception(std::format_string<Arguments...> const format,
		Arguments&&... arguments)
	{
		std::string const message{ std::format(format, std::forward<Arguments>(arguments)...) };
		Logger::error(message);
		throw std::runtime_error(message);
	}

	template <typename Type>
	[[noreturn]] void exception(Type&& message)
	{
		exception("{}", std::forward<Type>(message));
	}
};

#endif