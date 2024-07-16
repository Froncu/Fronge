#if not defined WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/Event.hpp"
#include "Maths/MathStructs.hpp"

namespace fro
{
	struct WindowCloseEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		std::uint32_t const ID;
	};

	struct WindowResizeEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		std::uint32_t const ID;
		Vector2<int> const size;
	};

	using WindowEvent = EventVariant<
		WindowCloseEvent,
		WindowResizeEvent>;
}

#endif