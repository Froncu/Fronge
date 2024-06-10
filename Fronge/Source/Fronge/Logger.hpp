#if not defined FRO_LOGGER_H
#define FRO_LOGGER_H

#include "Core.hpp"

#include <chrono>
#include <format>
#include <iostream>

namespace fro
{
	class FRO_API Logger final
	{
	public:
		template<typename ...Arguments>
		static void infoCore(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			log("37;40", "Fronge", format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void warnCore(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			log("1;33;40", "Fronge", format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void errorCore(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			log("1;31;40", "Fronge", format, std::forward<Arguments>(arguments)...);
		}

		template<typename Type>
		static void infoCore(Type&& value)
		{
			infoCore("{}", std::forward<Type>(value));
		}

		template<typename Type>
		static void warnCore(Type&& value)
		{
			warnCore("{}", std::forward<Type>(value));
		}

		template<typename Type>
		static void errorCore(Type&& value)
		{
			errorCore("{}", std::forward<Type>(value));
		}

		template<typename ...Arguments>
		static void info(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			log("37;40", "App", format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void warn(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			log("1;33;40", "App", format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void error(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			log("1;31;40", "App", format, std::forward<Arguments>(arguments)...);
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
		static void log(std::string_view const escSequence, std::string_view const loggerName,
			std::format_string<Arguments...> const format, Arguments&&... arguments)
		{
			auto const now{ std::chrono::system_clock::now() };

			std::cout
				<< std::format("\033[{}m", escSequence)
				<< std::format("[{:%H:%M:%S}] {}: ", std::chrono::floor<std::chrono::seconds>(now), loggerName)
				<< std::format(format, std::forward<Arguments>(arguments)...)
				<< "\033[0m\n";
		}
	};
}

#endif