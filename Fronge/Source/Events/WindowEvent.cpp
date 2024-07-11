#include "froch.hpp"

#include "WindowEvent.hpp"

namespace fro
{
	std::string WindowCloseEvent::getLogString() const
	{
		return std::format("close Window with ID {} event", ID);
	}

	FRO_API FRO_NODISCARD std::string WindowResizeEvent::getLogString() const
	{
		return std::format("resize Window with ID {} to {}x{} event", ID, width, height);
	}
}