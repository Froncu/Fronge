#if not defined EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include "froch.hpp"

// TODO: copy behaviour is disabled; what should happen when an Event is copied?

namespace fro
{
	template<typename...>
	class EventListener;

	template<typename... Payload>
	class Event final
	{
	public:
		using EventListenerType = EventListener<Payload...>;

		friend EventListenerType;

	public:
		Event() = default;
		Event(Event const&) = delete;
		Event(Event&& other) noexcept
			: mListeners{ std::move(other.mListeners) }
		{
			for (auto const listener : mListeners)
			{
				listener->mEvents.erase(&other);
				listener->mEvents.insert(this); 
			}
		}

		~Event()
		{
			for (auto const listener : mListeners)
				listener->mEvents.erase(this);
		};

		Event& operator=(Event const&) = delete;
		Event& operator=(Event&& other) noexcept
		{
			for (auto const listener : mListeners)
				listener->mEvents.erase(this);

			mListeners = std::move(other.mListeners);

			for (auto const listener : mListeners)
			{
				listener->mEvents.erase(&other);
				listener->mEvents.insert(this);
			}
		}

		void addListener(EventListenerType& eventListener)
		{
			eventListener.mEvents.insert(this);
			mListeners.insert(&eventListener);
		}

		void removeListener(EventListenerType& eventListener)
		{
			eventListener.mEvents.erase(this);
			mListeners.erase(&eventListener);
		}

		void notify(Payload... payload)
		{
			for (auto const listener : mListeners)
				listener->mOnNotify(std::forward<Payload>(payload)...);
		}

	private:
		std::set<EventListenerType*> mListeners;
	};
}

#endif