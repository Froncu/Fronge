#if not defined APPLICATION_HPP
#define APPLICATION_HPP

#include "Core.hpp"

#include <memory>

namespace fro
{
	class FRO_API Application
	{
	public:
		virtual ~Application() = default;

		virtual void run() = 0;

	protected:
		Application() = default;
		Application(Application const&) = default;
		Application(Application&&) noexcept = default;

		Application& operator=(Application const&) = default;
		Application& operator=(Application&&) noexcept = default;
	};
}

#endif