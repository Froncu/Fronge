#if not defined WINDOW_HPP
#define WINDOW_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/WindowEvent.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	class Window final : public Referencable
	{
		friend class Renderer;

		class Implementation;
		std::unique_ptr<Implementation> mImplementation;

	public:
		FRO_API Window(std::string_view const title = "Fronge", int const width = 640, int const height = 480);

		FRO_API ~Window();

		EventDispatcher<> mWindowCloseEvent{};
		EventDispatcher<> mWindowResizeEvent{};

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
				if (windowCloseEvent.ID == mID)
					if (mWindowCloseEvent.notify())
						return true;

				return false;
			}
		};

		EventListener<WindowResizeEvent&> mOnWindowResizeEvent
		{
			[this](auto&& windowResizeEvent)
			{
				if (windowResizeEvent.ID == mID)
				{
					mWidth = windowResizeEvent.width;
					mHeight = windowResizeEvent.height;

					if (mWindowResizeEvent.notify())
						return true;
				}

				return false;
			}
		};

		std::string_view mTitle;
		int mWidth;
		int mHeight;
	};
}

#endif