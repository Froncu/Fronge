#if not defined WINDOW_CLOSE_EVENT_HPP
#define WINDOW_CLOSE_EVENT_HPP

#include "WindowEvent.hpp"

namespace fro
{
	class WindowCloseEvent final : public WindowEvent
	{
	public:
		FRO_API WindowCloseEvent(std::uint32_t const ID);
		FRO_API WindowCloseEvent(WindowCloseEvent const& other);
		FRO_API WindowCloseEvent(WindowCloseEvent&& other) noexcept;

		FRO_API virtual ~WindowCloseEvent() override = default;

		FRO_API WindowCloseEvent& operator=(WindowCloseEvent const&) = delete;
		FRO_API WindowCloseEvent& operator=(WindowCloseEvent&&) noexcept = delete;
	};
}

#endif