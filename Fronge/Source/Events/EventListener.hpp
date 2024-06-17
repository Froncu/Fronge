#if not defined EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

#include "froch.hpp"

namespace fro
{
	template<typename...>
	class Event;

	template<typename... Payload>
	class EventListener final
	{
	public:
		using CallbackType = std::function<void(Payload...)>;

		friend Event<Payload...>;

	public:
		EventListener(CallbackType onNotify)
			: mOnNotify{ std::move(onNotify) }
		{
			FRO_ASSERT(mOnNotify not_eq nullptr, "the onNotify callback cannot be a nullptr!");
		}

		EventListener(EventListener const& other)
			: mOnNotify{ other.mOnNotify }
			, mEvents{ other.mEvents }
		{
			for (auto const dispatcher : mEvents)
				dispatcher->mListeners.insert(this);
		}

		EventListener(EventListener&& other) noexcept
			: mOnNotify{ std::move(other.mOnNotify) }
			, mEvents{ std::move(other.mEvents) }
		{
			for (auto const dispatcher : mEvents)
			{
				dispatcher->mListeners.erase(&other);
				dispatcher->mListeners.insert(this);
			}
		}

		~EventListener()
		{
			for (auto const dispatcher : mEvents)
				dispatcher->mListeners.erase(this);
		}

		EventListener& operator=(EventListener const& other)
		{
			if (this == &other)
				return *this;

			mOnNotify = other.mOnNotify;

			for (auto const dispatcher : mEvents)
				dispatcher->mListeners.erase(this);

			mEvents = other.mEvents;

			for (auto const dispatcher : mEvents)
				dispatcher->mListeners.insert(this);

			return *this;
		}

		EventListener& operator=(EventListener&& other) noexcept
		{
			if (this == &other)
				return *this;

			mOnNotify = std::move(other.mOnNotify);

			for (auto const dispatcher : mEvents)
				dispatcher->mListeners.erase(this);

			mEvents = std::move(other.mEvents);

			for (auto const dispatcher : mEvents)
			{
				dispatcher->mListeners.erase(&other);
				dispatcher->mListeners.insert(this);
			}

			return *this;
		}

	private:
		CallbackType mOnNotify;
		std::set<Event<Payload...>*> mEvents{};
	};
}

#endif