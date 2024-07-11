#include "froch.hpp"

#include "Logger.hpp"

namespace fro
{
	std::mutex Logger::sOutputMutex{};
}