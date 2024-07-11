#if not defined LOGGER_HPP
#define LOGGER_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	class Logger final
	{
	public:
#if defined FRO_ENGINE
		static std::string_view constexpr LOGGER_NAME{ "FRONGE" };
#else
		static std::string_view constexpr LOGGER_NAME{ "APP" };
#endif

		template<typename ...Arguments>
		static void info(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			if constexpr (LOGGER_NAME == "FRONGE")
				log("1;2;37;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
			else
				log("1;37;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void warn(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			if constexpr (LOGGER_NAME == "FRONGE")
				log("1;2;33;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
			else
				log("1;33;40", LOGGER_NAME, format, std::forward<Arguments>(arguments)...);
		}

		template<typename ...Arguments>
		static void error(std::format_string<Arguments...> const format,
			Arguments&&... arguments)
		{
			if constexpr (LOGGER_NAME == "FRONGE")
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
		template<typename ...Arguments>
		static void log([[maybe_unused]] std::string_view const escSequence, [[maybe_unused]] std::string_view const loggerName,
			[[maybe_unused]] std::format_string<Arguments...> const format, [[maybe_unused]] Arguments&&... arguments)
		{
#if defined FRO_CONSOLE
			auto const now{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
			std::tm localTime;
			localtime_s(&localTime, &now);
			
			std::ostringstream timeStream;
			timeStream << std::put_time(&localTime, "%H:%M:%S");
			
			std::lock_guard const lock{ sOutputMutex };
			std::cout
				<< std::format("\033[{}m", escSequence)
				<< std::format("[{}] {}: ", timeStream.str(), loggerName)
				<< std::format(format, std::forward<Arguments>(arguments)...)
				<< "\033[0m\n";
#endif
		}
		
		FRO_API static std::mutex sOutputMutex;

		Logger() = delete;
		Logger(Logger const&) = delete;
		Logger(Logger&&) noexcept = delete;

		~Logger() = delete;

		Logger& operator=(Logger const&) = delete;
		Logger& operator=(Logger&&) noexcept = delete;
	};
}

#endif