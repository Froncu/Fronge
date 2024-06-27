#if not defined WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/Event.hpp"

namespace fro
{
	struct WindowCloseEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		std::uint32_t const ID;
	};

	using WindowEvent = EventVariant<WindowCloseEvent>;
}

#endif