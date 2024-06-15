#if not defined WINDOW_HPP
#define WINDOW_HPP

#include "Core.hpp"

#include <memory>
#include <string>

namespace fro
{
	class Window final
	{
	public:
		FRO_API Window(std::string title = "Fronge", int const width = 640, int const height = 480);

		FRO_API ~Window();

	private:
		Window(Window const&) = delete;
		Window(Window&) noexcept = delete;

		Window& operator=(Window const&) = delete;
		Window& operator=(Window&) noexcept = delete;

		struct Data final
		{
			std::string title;
			int width;
			int height;
		};

		Data mData;

		class Implementation;
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif