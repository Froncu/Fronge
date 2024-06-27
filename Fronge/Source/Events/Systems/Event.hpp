#if not defined EVENT_HPP
#define EVENT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "EventDispatcher.hpp"
#include "EventListener.hpp"
#include "UniqueEventQueue.hpp"

namespace fro
{
	template<typename Type>
	concept EventType = requires(Type event)
	{
		{ event.getLogString() } -> std::same_as<std::string>;
	};

	template<EventType... Types>
	using EventVariant = std::variant<Types...>;
}

#endif