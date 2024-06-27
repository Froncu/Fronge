#include "froch.hpp"

#include "WindowEvent.hpp"

namespace fro
{
	std::string WindowCloseEvent::getLogString() const
	{
		return std::format("window with ID {} close event", ID);
	}
}