#if not defined EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

#include "froch.hpp"

namespace fro
{
	template<typename...>
	class EventDispatcher;

	template<typename... Payload>
	class EventListener final
	{
	public:
		using CallbackType = std::function<bool(Payload...)>;

		friend EventDispatcher<Payload...>;

	public:
		EventListener(CallbackType onNotify)
			: mOnNotify{ std::move(onNotify) }
		{
			FRO_ASSERT(mOnNotify not_eq nullptr, "the onNotify callback cannot be a nullptr!");
		}

		EventListener(EventListener const& other)
			: mOnNotify{ other.mOnNotify }
			, mDispatchers{ other.mDispatchers }
		{
			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.insert(this);
		}

		EventListener(EventListener&& other) noexcept
			: mOnNotify{ std::move(other.mOnNotify) }
			, mDispatchers{ std::move(other.mDispatchers) }
		{
			for (auto const dispatcher : mDispatchers)
			{
				dispatcher->mListeners.erase(&other);
				dispatcher->mListeners.insert(this);
			}
		}

		~EventListener()
		{
			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.erase(this);
		}

		EventListener& operator=(EventListener const& other)
		{
			if (this == &other)
				return *this;

			mOnNotify = other.mOnNotify;

			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.erase(this);

			mDispatchers = other.mDispatchers;

			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.insert(this);

			return *this;
		}

		EventListener& operator=(EventListener&& other) noexcept
		{
			if (this == &other)
				return *this;

			mOnNotify = std::move(other.mOnNotify);

			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.erase(this);

			mDispatchers = std::move(other.mDispatchers);

			for (auto const dispatcher : mDispatchers)
			{
				dispatcher->mListeners.erase(&other);
				dispatcher->mListeners.insert(this);
			}

			return *this;
		}

		bool operator()(Payload... payload) const
		{
			return mOnNotify(std::forward<Payload>(payload)...);
		}

	private:
		CallbackType mOnNotify;
		std::set<EventDispatcher<Payload...>*> mDispatchers{};
	};
}

#endif