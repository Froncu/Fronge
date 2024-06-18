#if not defined WINDOW_HPP
#define WINDOW_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Events.hpp"
#include "Events/Polymorphic/WindowCloseEvent.hpp"

namespace fro
{
	class Window final
	{
		class Implementation;
		std::unique_ptr<Implementation> mImplementation;

	public:
		FRO_API Window(std::string_view const title = "Fronge", int const width = 640, int const height = 480);

		FRO_API ~Window();

		EventDispatcher<> mWindowCloseEvent{};

		std::uint32_t const mID;

	private:
		Window(Window const&) = delete;
		Window(Window&) noexcept = delete;

		Window& operator=(Window const&) = delete;
		Window& operator=(Window&) noexcept = delete;

		EventListener<WindowCloseEvent&> mOnWindowCloseEvent
		{
			[this](auto&& windowCloseEvent)
			{
				if (windowCloseEvent.mID == mID)
					if (mWindowCloseEvent.notify())
						return true;

				return false;
			}
		};

		std::string_view mTitle;
		int mWidth;
		int mHeight;
	};
}

#endif