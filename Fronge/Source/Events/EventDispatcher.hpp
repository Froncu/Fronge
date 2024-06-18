#if not defined EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include "froch.hpp"

// TODO: copy behaviour is disabled; what should happen when an EventDispatcher is copied?

namespace fro
{
	template<typename...>
	class EventListener;

	template<typename... Payload>
	class EventDispatcher final
	{
	public:
		using EventListenerType = EventListener<Payload...>;

		friend EventListenerType;

	public:
		EventDispatcher() = default;
		EventDispatcher(EventDispatcher const&) = delete;
		EventDispatcher(EventDispatcher&& other) noexcept
			: mListeners{ std::move(other.mListeners) }
		{
			for (auto const listener : mListeners)
			{
				listener->mDispatchers.erase(&other);
				listener->mDispatchers.insert(this); 
			}
		}

		~EventDispatcher()
		{
			for (auto const listener : mListeners)
				listener->mDispatchers.erase(this);
		};

		EventDispatcher& operator=(EventDispatcher const&) = delete;
		EventDispatcher& operator=(EventDispatcher&& other) noexcept
		{
			for (auto const listener : mListeners)
				listener->mDispatchers.erase(this);

			mListeners = std::move(other.mListeners);

			for (auto const listener : mListeners)
			{
				listener->mDispatchers.erase(&other);
				listener->mDispatchers.insert(this);
			}
		}

		void addListener(EventListenerType& eventListener)
		{
			eventListener.mDispatchers.insert(this);
			mListeners.insert(&eventListener);
		}

		void removeListener(EventListenerType& eventListener)
		{
			eventListener.mDispatchers.erase(this);
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