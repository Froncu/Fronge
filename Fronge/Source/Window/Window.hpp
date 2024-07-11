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
		class Implementation;

	public:
		FRO_API Window(std::string_view const title = "Fronge Window", int const width = 640, int const height = 480);

		FRO_API ~Window();

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD std::uint32_t getID() const;
		FRO_API FRO_NODISCARD int getWidth() const;
		FRO_API FRO_NODISCARD int getHeight() const;

		EventDispatcher<> mWindowCloseEvent{};
		EventDispatcher<> mWindowResizeEvent{};

	private:
		Window(Window const&) = delete;
		Window(Window&&) noexcept = delete;

		Window& operator=(Window const&) = delete;
		Window& operator=(Window&&) noexcept = delete;

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
		std::unique_ptr<Implementation> mImplementation;
		std::uint32_t mID;
	};
}

#endif