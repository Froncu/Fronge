#if not defined WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "Event.hpp"

namespace fro
{
	class WindowEvent : public Event
	{
	public:
		std::uint32_t const mID;

	protected:
		WindowEvent(std::string_view const name, Type const type, std::uint32_t const ID);
		WindowEvent(WindowEvent const& other);
		WindowEvent(WindowEvent&& other) noexcept;

		virtual ~WindowEvent() override = default;

		virtual std::string getLogString() const override;

	private:
		WindowEvent& operator=(WindowEvent const&) = delete;
		WindowEvent& operator=(WindowEvent&&) noexcept = delete;
	};
}

#endif