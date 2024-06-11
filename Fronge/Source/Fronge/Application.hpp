#if not defined fro_APPLICATION_HPP
#define fro_APPLICATION_HPP

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

	std::unique_ptr<Application> createApplication();
}

#endif