#if not defined WINDOW_HPP
#define WINDOW_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Events/WindowEvent.hpp"
#include "Maths/MathStructs.hpp"
#include "Reference/Referencable.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
	class Window final : public Referencable
	{
		class Implementation;

	public:
		FRO_API Window(std::string_view const title = "Fronge Window", Vector2<int> const size = { 640, 480 });

		FRO_API ~Window();

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API bool setFullscreen(bool const fullscreen);
		
		FRO_API FRO_NODISCARD std::uint32_t getID() const;
		FRO_API FRO_NODISCARD Vector2<int> getSize() const;

		EventDispatcher<> mCloseEvent{};
		EventDispatcher<Vector2<int> const> mResizeEvent{};

	private:
		Window(Window const&) = delete;
		Window(Window&&) noexcept = delete;

		Window& operator=(Window const&) = delete;
		Window& operator=(Window&&) noexcept = delete;

		EventListener<WindowEvent const> mOnWindowEvent
		{
			VariantVisitor
			{
				[this](WindowCloseEvent const& windowCloseEvent)
				{
					if (windowCloseEvent.ID not_eq mID)
						return false;

					return mCloseEvent.notify();
				},

				[this](WindowResizeEvent const& windowResizeEvent)
				{
					if (windowResizeEvent.ID not_eq mID)
						return false;

					mSize = windowResizeEvent.size;
					mResizeEvent.notify(mSize);
					return true;
				}
			}
		};

		std::string_view mTitle;
		Vector2<int> mSize;
		std::unique_ptr<Implementation> mImplementation;
		std::uint32_t mID;
	};
}

#endif