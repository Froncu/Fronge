#if not defined WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "EventSystem/Event.hpp"

namespace fro
{
	class WindowEvent : public Event
	{
	public:
		std::uint32_t const mID;

		virtual std::string getLogString() const override;

	protected:
		WindowEvent(std::string_view const name, Type const type, std::uint32_t const ID);
		WindowEvent(WindowEvent const& other);
		WindowEvent(WindowEvent&& other) noexcept;

		virtual ~WindowEvent() override = default;

	private:
		WindowEvent& operator=(WindowEvent const&) = delete;
		WindowEvent& operator=(WindowEvent&&) noexcept = delete;
	};
}

#endif