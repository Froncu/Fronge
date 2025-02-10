#ifndef ASSERT_HPP
#define ASSERT_HPP

#include "Logger/Logger.hpp"

namespace fro
{
	template <typename... Arguments>
	void assert([[maybe_unused]] bool const condition,
		[[maybe_unused]] std::format_string<Arguments...> const format,
		[[maybe_unused]] Arguments&&... arguments)
	{
#ifndef NDEBUG
		if (condition)
			return;

		fro::Logger::error(format, std::forward<Arguments>(arguments)...);
		__debugbreak();
#endif
	}

	template <typename Type>
	void assert(bool const condition, Type&& message)
	{
		assert(condition, "{}", std::forward<Type>(message));
	}
}

#endif