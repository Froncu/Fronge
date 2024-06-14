#if not defined LOGGER_HPP
#define LOGGER_HPP

#include "Core.hpp"

#include <chrono>
#include <format>
#include <iostream>

#if defined FRO_ENGINE
	#define LOGGER_NAME "Fronge"
#else
	#define LOGGER_NAME "App"
#endif

namespace fro
{
	class Logger final
	{
	public:
		template<typename ...Arguments>
		static void info(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			if constexpr (*LOGGER_NAME == *"Fronge")
				log("1;2;37;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
			else
				log("1;37;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void warn(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			if constexpr (*LOGGER_NAME == *"Fronge")
				log("1;2;33;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
			else
				log("1;33;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void error(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			if constexpr (*LOGGER_NAME == *"Fronge")
				log("1;2;31;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
			else
				log("1;31;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
		}

		template<typename Type>
		static void info(Type&& value)
		{
			info("{}", std::forward<Type>(value));
		}

		template<typename Type>
		static void warn(Type&& value)
		{
			warn("{}", std::forward<Type>(value));
		}

		template<typename Type>
		static void error(Type&& value)
		{
			error("{}", std::forward<Type>(value));
		}

	private:
		Logger() = delete;
		Logger(Logger const&) = delete;
		Logger(Logger&&) noexcept = delete;

		~Logger() = delete;

		Logger& operator=(Logger const&) = delete;
		Logger& operator=(Logger&&) noexcept = delete;

		template<typename ...Arguments>
		static void log([[maybe_unused]] std::string_view const escSequence, [[maybe_unused]] std::string_view const loggerName,
			[[maybe_unused]] std::format_string<Arguments...> const format, [[maybe_unused]] Arguments&&... arguments)
		{
#if not defined FRO_DISTRIBUTE
			auto const now
			{
				std::chrono::zoned_time(std::chrono::current_zone(),
					std::chrono::system_clock::now()).get_local_time()
			};

			std::cout
				<< std::format("\033[{}m", escSequence)
				<< std::format("[{:%H:%M:%S}] {}: ", std::chrono::floor<std::chrono::seconds>(now), loggerName)
				<< std::format(format, std::forward<Arguments>(arguments)...)
				<< "\033[0m\n";
#endif
		}
	};
}

#undef LOGGER_NAME

#endif