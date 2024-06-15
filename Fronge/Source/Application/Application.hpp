#if not defined APPLICATION_HPP
#define APPLICATION_HPP

#include "../Core.hpp"
#include "Window/Window.hpp"

#include <memory>

namespace fro
{
	class Application
	{
	public:
		FRO_API virtual ~Application() = default;

		FRO_API void run();

	protected:
		Application() = default;
		Application(Application const&) = default;
		Application(Application&&) noexcept = default;

		Application& operator=(Application const&) = default;
		Application& operator=(Application&&) noexcept = default;

	private:
		Window mWindow{};
	};
}

#endif