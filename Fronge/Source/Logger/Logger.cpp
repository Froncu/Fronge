#include "froch.hpp"

#include "Logger.hpp"

namespace fro
{
	std::mutex Logger::sOutputMutex{};

	void Logger::separator()
	{
		std::lock_guard const lock{ sOutputMutex };
		std::cout << "\n---\n\n";
	}
}