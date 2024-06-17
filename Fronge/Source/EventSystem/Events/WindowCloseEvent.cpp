#include "WindowCloseEvent.hpp"

namespace fro
{
	WindowCloseEvent::WindowCloseEvent(std::uint32_t const ID)
		: WindowEvent("Window close event", Type::WINDOW_CLOSE, ID)
	{
	}

	WindowCloseEvent::WindowCloseEvent(WindowCloseEvent const& other)
		: WindowEvent(other)
	{
	}
	WindowCloseEvent::WindowCloseEvent(WindowCloseEvent&& other) noexcept
		: WindowEvent(std::move(other))
	{
	}
}