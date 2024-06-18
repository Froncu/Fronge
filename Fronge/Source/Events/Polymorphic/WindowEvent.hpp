#if not defined WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "BaseEvent.hpp"

namespace fro
{
	class WindowEvent : public BaseEvent
	{
	public:
		std::uint32_t const mID;

		FRO_NODISCARD FRO_API virtual std::string getLogString() const override;

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