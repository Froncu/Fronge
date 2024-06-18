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

		virtual ~WindowCloseEvent() override = default;

		WindowCloseEvent& operator=(WindowCloseEvent const&) = delete;
		WindowCloseEvent& operator=(WindowCloseEvent&&) noexcept = delete;
	};
}

#endif