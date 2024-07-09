#include "froch.hpp"

#include "WindowEvent.hpp"

namespace fro
{
	std::string WindowCloseEvent::getLogString() const
	{
		return std::format("window with ID {} close event", ID);
	}

	FRO_API FRO_NODISCARD std::string WindowResizeEvent::getLogString() const
	{
		return std::format("window with ID {} resized to {}x{} event", ID, width, height);
	}
}